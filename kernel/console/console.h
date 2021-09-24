#ifndef __CONSOLE_H_
#define __CONSOLE_H_

    #include <defs.h>		

    typedef enum realcolor{
        BLACK = 0,
        BLUE = 1,
        green = 2,
        ORANGE = 3,
        RED = 4,
        magenta = 5,
        brown = 6,
        light_grey = 7,
        dark_grey = 8,
        INDIGO = 9,
        light_green = 10,
        light_cyan = 11,
        light_red = 12,
        light_magenta = 13,
        light_brown  = 14,   // yellow
        WHITE = 15
    }realcolor_t;

    //clear screen
    void console_clear();

    //screen output a colored letter
    void console_putc_color(char c,realcolor_t back,realcolor_t fore);

    //screen output a str,black background,write font,end of '\0'
    void console_write(char* c_str);

    //screen output a str,end of '\0',with color
    void console_write_color(char* c_str,realcolor_t back,realcolor_t fore);

    //screen output a colored hex
    void console_write_hex(uint32_t n,realcolor_t back,realcolor_t fore);    

    //screen output a colored decimal
    void console_write_dec(uint32_t n,realcolor_t back,realcolor_t fore);

    void console_init();
#endif