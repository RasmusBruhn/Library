#ifndef FILES_H_DEFINED
#define FILES_H_DEFINED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

// Errors
#define ERR_PREFIX FIL
#include <Error.h>

enum ___FIL_ErrorID {
    _FIL_ERRORID_NONE = 0x400000000,
    _FIL_ERRORID_LOAD_FILE = 0x400010200,
    _FIL_ERRORID_LOAD_SEEKEND = 0x400010201,
    _FIL_ERRORID_LOAD_FTELL = 0x400010202,
    _FIL_ERRORID_LOAD_MALLOC = 0x400010203,
    _FIL_ERRORID_LOAD_REALLOC = 0x400010204,
    _FIL_ERRORID_SAVECSV_MALLOCVAR = 0x400020200,
    _FIL_ERRORID_SAVECSV_MALLOCTYPE = 0x400020201,
    _FIL_ERRORID_SAVECSV_TYPE = 0x400020202,
    _FIL_ERRORID_SAVECSV_FILE = 0x400020203,
    _FIL_ERRORID_SAVECSV_HEADER = 0x400020204,
    _FIL_ERRORID_SAVECSV_NUMBER = 0x400020205
};

#define _FIL_ERRORMES_MALLOC "Unable to allocate memory (Size: %lu)"
#define _FIL_ERRORMES_REALLOC "Unable to reallocate memory (Size: %lu)"
#define _FIL_ERRORMES_OPENFILE "Unable to open file \"%s\""
#define _FIL_ERRORMES_FILESEEK "Unable to set position in file (Pos: %s, Offset: %ld)"
#define _FIL_ERRORMES_FILETELL "Unable to get position in file"
#define _FIL_ERRORMES_FILEREAD "Unable to read file (Size: %lu)"
#define _FIL_ERRORMES_UNKNOWNTYPE "Unknown data type (ID: %u)"
#define _FIL_ERRORMES_WRITEHEADER "Unable to write header to file \"%s\""
#define _FIL_ERRORMES_WRITENUMBER "Unable to write number to file \"%s\""

#define STR(Macro) #Macro

enum __FIL_DataType {
    FIL_DATATYPE_NONE,
    FIL_DATATYPE_BOOL,
    FIL_DATATYPE_UINT8,
    FIL_DATATYPE_UINT16,
    FIL_DATATYPE_UINT32,
    FIL_DATATYPE_UINT64,
    FIL_DATATYPE_INT8,
    FIL_DATATYPE_INT16,
    FIL_DATATYPE_INT32,
    FIL_DATATYPE_INT64,
    FIL_DATATYPE_FLOAT,
    FIL_DATATYPE_DOUBLE
};

typedef enum ___FIL_ErrorID _FIL_ErrorID;
typedef enum __FIL_DataType FIL_DataType;
typedef enum __FIL_DataTypeBase FIL_DataTypeBase;
typedef union __FIL_Value FIL_Value;

union __FIL_Value {
    uint64_t u;
    int64_t i;
    double f;
};

#define FIL_FORMAT_INT ((sizeof(long int) == 4) ? ("%s%lld") : ("%s%ld"))
#define FIL_FORMAT_UINT ((sizeof(long int) == 4) ? ("%s%llu") : ("%s%lu"))
#define FIL_FORMAT_FLOAT "%s%g"
#define FIL_FORMAT_NEWLINE "\n"

// Loads the entirety of the file into a string
// FileName: The path of the file
char *FIL_Load(const char *FileName);

// Loads a csv file, returns false on error
// FileName: The path of the file
// Header: The header to write, does not include the final newline character
// Delimiter: The delimiter between each element
// Length: The length of the lists
// Count: The number of elements to write per line
// ...: [Pointer to array long enough to hold all values, then the size of the elements]
bool FIL_SaveCSV(const char *FileName, const char *Header, const char *Delimiter, size_t Length, size_t Count, ...);

char *FIL_Load(const char *FileName)
{
    // Open the file
    FILE *File = fopen(FileName, "r");

    if (File == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_LOAD_FILE, strerror(errno), _FIL_ERRORMES_OPENFILE, FileName);
        return NULL;
    }

    // Get length of file
    if (fseek(File, 0, SEEK_END) != 0)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_LOAD_SEEKEND, strerror(errno), _FIL_ERRORMES_FILESEEK, STR(SEEK_END), 0);
        fclose(File);
        return NULL;
    }

    int64_t Size = ftell(File);

    if (Size == -1)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_LOAD_FTELL, strerror(errno), _FIL_ERRORMES_FILETELL);
        fclose(File);
        return NULL;
    }

    rewind(File);
    
    // Allocate memory
    char *String = (char *)malloc(sizeof(char) * (Size + 1));
    
    if (String == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_LOAD_MALLOC, strerror(errno), _FIL_ERRORMES_MALLOC, sizeof(char) * (Size + 1));
        fclose(File);
        return NULL;
    }
    
    // Read file
    Size = fread(String, sizeof(char), Size, File);

    // Resize to make up for newlines
    String = (char *)realloc(String, sizeof(char) * (Size + 1));

    if (String == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_LOAD_REALLOC, strerror(errno), _FIL_ERRORMES_REALLOC, sizeof(char) * (Size + 1));
        fclose(File);
        return NULL;
    }

    String[Size] = '\0';
    fclose(File);
    
    return String;
}

bool FIL_SaveCSV(const char *FileName, const char *Header, const char *Delimiter, size_t Length, size_t Count, ...)
{
    // Allocate memory for list of lists and types
    void **VariableList = (void **)malloc(sizeof(void *) * Count);

    if (VariableList == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_SAVECSV_MALLOCVAR, strerror(errno), _FIL_ERRORMES_MALLOC, sizeof(void *) * Count);
        return false;
    }

    FIL_DataType *TypeList = (FIL_DataType *)malloc(sizeof(FIL_DataType) * Count);

    if (TypeList == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_SAVECSV_MALLOCTYPE, strerror(errno), _FIL_ERRORMES_MALLOC, sizeof(FIL_DataType) * Count);
        free(VariableList);
        return false;
    }

    // Get the variable parameters
    va_list Args;
    va_start(Args, Count);

    for (size_t Pos = 0; Pos < Count; ++Pos)
    {
        VariableList[Pos] = va_arg(Args, void *);
        TypeList[Pos] = va_arg(Args, FIL_DataType);
    }

    va_end(Args);

    // Open the file
    FILE *File = fopen(FileName, "w");

    if (File == NULL)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_SAVECSV_FILE, strerror(errno), _FIL_ERRORMES_OPENFILE, FileName);
        free(VariableList);
        free(TypeList);
        return false;
    }

    // Write the header
    if (fprintf(File, "%s", Header) < 0)
    {
        _FIL_AddErrorForeign(_FIL_ERRORID_SAVECSV_HEADER, strerror(errno), _FIL_ERRORMES_WRITEHEADER, FileName);
        free(VariableList);
        free(TypeList);
        fclose(File);
        return false;
    }

    // Go through all of the lines
    for (size_t Line = 0; Line < Length; ++Line)
    {
        // Write each element
        FIL_DataType *TempTypeList = TypeList;

        for (void **TempVariableList = VariableList, **EndTempVariableList = VariableList + Count; TempVariableList < EndTempVariableList;++TempTypeList, ++TempVariableList)
        {
            // Get the delimiter string
            const char *Delim = ((TempVariableList == VariableList) ? (FIL_FORMAT_NEWLINE) : (Delimiter));

            // Get the value
            FIL_Value Value;
            const char *Format;

            switch (*TempTypeList)
            {
                case (FIL_DATATYPE_BOOL):
                    Value.u = (uint64_t)*((bool *)*TempVariableList);
                    *TempVariableList += sizeof(bool);
                    Format = FIL_FORMAT_UINT;
                    break;
                    
                case (FIL_DATATYPE_UINT8):
                    Value.u = (uint64_t)*((uint8_t *)*TempVariableList);
                    *TempVariableList += sizeof(uint8_t);
                    Format = FIL_FORMAT_UINT;
                    break;
                    
                case (FIL_DATATYPE_UINT16):
                    Value.u = (uint64_t)*((uint16_t *)*TempVariableList);
                    *TempVariableList += sizeof(uint16_t);
                    Format = FIL_FORMAT_UINT;
                    break;
                    
                case (FIL_DATATYPE_UINT32):
                    Value.u = (uint64_t)*((uint32_t *)*TempVariableList);
                    *TempVariableList += sizeof(uint32_t);
                    Format = FIL_FORMAT_UINT;
                    break;
                    
                case (FIL_DATATYPE_UINT64):
                    Value.u = (uint64_t)*((uint64_t *)*TempVariableList);
                    *TempVariableList += sizeof(uint64_t);
                    Format = FIL_FORMAT_UINT;
                    break;
                    
                case (FIL_DATATYPE_INT8):
                    Value.i = (int64_t)*((int8_t *)*TempVariableList);
                    *TempVariableList += sizeof(int8_t);
                    Format = FIL_FORMAT_INT;
                    break;
                    
                case (FIL_DATATYPE_INT16):
                    Value.i = (int64_t)*((int16_t *)*TempVariableList);
                    *TempVariableList += sizeof(int16_t);
                    Format = FIL_FORMAT_INT;
                    break;
                    
                case (FIL_DATATYPE_INT32):
                    Value.i = (int64_t)*((int32_t *)*TempVariableList);
                    *TempVariableList += sizeof(int32_t);
                    Format = FIL_FORMAT_INT;
                    break;
                    
                case (FIL_DATATYPE_INT64):
                    Value.i = (int64_t)*((int64_t *)*TempVariableList);
                    *TempVariableList += sizeof(int64_t);
                    Format = FIL_FORMAT_INT;
                    break;
                    
                case (FIL_DATATYPE_FLOAT):
                    Value.f = (double)*((float *)*TempVariableList);
                    *TempVariableList += sizeof(float);
                    Format = FIL_FORMAT_FLOAT;
                    break;
                    
                case (FIL_DATATYPE_DOUBLE):
                    Value.f = (double)*((double *)*TempVariableList);
                    *TempVariableList += sizeof(double);
                    Format = FIL_FORMAT_FLOAT;
                    break;
                
                default:
                    _FIL_SetError(_FIL_ERRORID_SAVECSV_TYPE, _FIL_ERRORMES_UNKNOWNTYPE, *TempTypeList);
                    free(VariableList);
                    free(TypeList);
                    fclose(File);
                    return false;
            }

            // Print
            if (fprintf(File, Format, Delim, Value, Value) < 0)
            {
                    _FIL_AddErrorForeign(_FIL_ERRORID_SAVECSV_NUMBER, strerror(errno), _FIL_ERRORMES_WRITENUMBER, FileName);
                    free(VariableList);
                    free(TypeList);
                    fclose(File);
            }
        }
    }

    free(VariableList);
    free(TypeList);
    fclose(File);

    return true;
}

#endif