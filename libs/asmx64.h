#ifndef __ASMX64_H_
#define __ASMX64_H_
    #include <defs.h>

    static inline void outb(uint16_t port,uint8_t value){
        asm volatile("outb %1,%0"::"dN"(port),"a"(value));
    }

    static inline uint8_t inb(uint16_t port){
        uint8_t ret;
        asm volatile("inb %1,%0":"=a"(ret):"dN"(port));
        return ret;
    }

    static inline uint16_t inw(uint16_t port){
        uint16_t ret;
        asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
        return ret;
    }

    static inline void sti(){
        asm volatile("sti");
    }

    static inline void cli(){
        asm volatile("cli");
    }
    
#endif