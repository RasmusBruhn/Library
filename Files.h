#ifndef FILES_H_DEFINED
#define FILES_H_DEFINED

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

// Errors
#define ERR_PREFIX FIL
#include <Error.h>

enum _FIL_ErrorID {
    _FIL_ERRORID_NONE = 0x400000000,
    _FIL_ERRORID_LOAD_FILE = 0x400010200,
    _FIL_ERRORID_LOAD_SEEKEND = 0x400010201,
    _FIL_ERRORID_LOAD_FTELL = 0x400010202,
    _FIL_ERRORID_LOAD_MALLOC = 0x400010203,
    _FIL_ERRORID_LOAD_REALLOC = 0x400010204
};

#define _FIL_ERRORMES_MALLOC "Unable to allocate memory (Size: %lu)"
#define _FIL_ERRORMES_REALLOC "Unable to reallocate memory (Size: %lu)"
#define _FIL_ERRORMES_OPENFILE "Unable to open file \"%s\""
#define _FIL_ERRORMES_FILESEEK "Unable to set position in file (Pos: %s, Offset: %ld)"
#define _FIL_ERRORMES_FILETELL "Unable to get position in file"
#define _FIL_ERRORMES_FILEREAD "Unable to read file (Size: %lu)"

#define STR(Macro) #Macro

// Loads the entirety of the file into a string
// FileName: The path of the file
char *FIL_Load(const char *FileName);

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

#endif