#include <tstring.h>

inline void memcpy(uint8_t* dest,uint8_t* src,uint32_t len){
    for(int i=0;i<len;i++){
        *(dest+i) = *(src+i);
    }
} 

inline void memcopy(void* dest,const void* src,size_t n){
    uint8_t* dd = (uint8_t*)dest;
    uint8_t* ss = (uint8_t*)src;
    memcpy(dd,ss,n);
}

inline void memset(void* dest,uint8_t val,uint32_t len){
    //transform to uint8_t* type
    uint8_t* dst = (uint8_t*)dest;
    for(int i=0;i<len;i++){
        *(dst+i) = val;
    }
}

inline void bzero(void* dest,uint32_t len){
    memset(dest,0,len);
}

inline int strcmp(const char* s1,const char* s2){
    int i;
    for(i=0;s1[i] != '\0' && s2[i] != '\0';i++){
        if(s1[i] != s2[i]) return 1;
    }
    if(s1[i] == s2[i]) return 0;
    return 1;
}

inline char* strcpy(char* dest,const char* src){
    uint8_t* dst = (uint8_t*)dest;
    uint8_t* ssrc = (uint8_t*)src;
    memcpy(dst,ssrc,strlen(src));
    return dest;
}

inline char* strcat(char* dest,const char* src){
    int l = strlen(dest);
    int j = 0;
    while(src[j] != '\0') {
        *(dest+j+l) = src[j];
        j++;
    }
    *(dest+j+l) = '\0';
    return dest;
}

inline int strlen(const char* src){
    int cnt = 0;
    while(src[cnt] != '\0') cnt++;
    return cnt;
}