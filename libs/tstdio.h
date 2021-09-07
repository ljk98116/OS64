#ifndef __TSTDIO_H_
#define __TSTDIO_H_
    #include <tstdarg.h>
    #include <tstring.h>
    #include <console.h>

    void printk(const char *format,...);
    void printk_color(uint32_t back,uint32_t fore,const char *format,...);
#endif