#ifndef __TSTRING_H_
#define __TSTRING_H_
    #include <defs.h>
    
    void memcpy(uint8_t* dest,uint8_t* src,uint32_t len);
    void memcopy(void* dest,const void* src,size_t n);
    void memset(void* dest,uint8_t val,uint32_t len);

    void bzero(void* dest,uint32_t len);

    //same return 0,or return 1
    int strcmp(const char* s1,const char* s2);

    char* strcpy(char* dest,const char* src);

    char* strcat(char* dest,const char* src);

    int strlen(const char* src);    
#endif