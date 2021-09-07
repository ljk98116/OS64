#ifndef __CONSOLE_H_
#define __CONSOLE_H_

    #include <defs.h>

    #define WHITE 	0x00ffffff		
    #define BLACK 	0x00000000		
    #define RED	0x00ff0000		
    #define ORANGE	0x00ff8000		
    #define YELLOW	0x00ffff00		
    #define GREEN	0x0000ff00		
    #define BLUE	0x000000ff		
    #define INDIGO	0x0000ffff		
    #define PURPLE	0x008000ff		

    typedef struct position{
        int XRes,YRes;
        int XPos,YPos;
        int XCharSize,YCharSize;
        uint32_t *FB_addr;
        uint32_t FB_len;
    }Pos;

    void console_init();
    void console_clear();
    void console_putc_color(char c,uint32_t back,uint32_t fore);
    void console_write_color(char* str,uint32_t back,uint32_t fore);
    void console_write(char *str);
#endif