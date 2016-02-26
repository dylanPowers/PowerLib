#ifndef TYPES_H
#define TYPES_H

typedef unsigned char    u8;
typedef unsigned short   u16;

typedef unsigned short   ushort;
typedef unsigned int     uint;
typedef unsigned long    ulong;

#ifdef __BCC__1 // 16bit
typedef unsigned long    u32;

typedef enum {
  false = 0, true = 1
} bool;

#define NULL 0

#else // 32/64bit
typedef unsigned int     u32;

#endif


#endif
