#ifndef DEFINES_H
#define DEFINES_H

// Correct print for 64 bit integers
#ifdef _WIN32
#define PRINT_UINT64 "%llu"
#define PRINT_INT64 "%lld"
#else
#define PRINT_UINT64 "%lu"
#define PRINT_INT64 "%ld"
#endif

#endif