#ifndef __TYPE_H_
#define __TYPE_H_
    // data types
    typedef char int8_t;
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef unsigned short uint16_t;
    typedef int int32_t;
    typedef unsigned int uint32_t;
    typedef long  int64_t;
    typedef unsigned long uint64_t;
    typedef uint32_t size_t;
    typedef uint32_t uintptr_t;
    
    #ifndef NULL
        #define NULL 0
    #endif
    
    #define isdigit(c) (c >='0' && c <='9')
#endif