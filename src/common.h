#ifndef COMMON_H
#define COMMON_H

#define VOID_POINTER(offset) ((const GLvoid *)(offset))
#define ARRAY_ELEMENTS(a) ( sizeof(a)/sizeof(a[0]) )
#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }
#define SAFE_DELETEV(p) if (p) { delete[] p; p = nullptr; }
#define MAX_INFO_LOG_SIZE 2048

typedef unsigned int uint;
typedef uint16_t ushort;
typedef unsigned char ubyte;

#endif
