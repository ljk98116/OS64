#include <console.h>
#include <asmx64.h>

//ascii in low byte
//color in high byte

/*
    attribute byte
    bit 0~2 font color: 0:black 1:blue 2:green 3:light blue 4:red 5:purple 6:brown 7:white
    bit 3 lightness: 0:normal 1:high light
    bit 4~6 background color: same as font color
    bit 7 shining: 0:not 1:yes
*/

//VGA buffer begins at 0xB8000
static uint16_t* vga_base = (uint16_t*)(0xB8000);

//screen cursor loc
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

//move cursor
static void move_cursor();

//scroll screen
static void scroll();

void console_clear(){
    //using blank to fill in the screen
    //white background black font
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    for (int i = 0; i < 80 * 25; i++) {
        vga_base[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();    
}

void console_putc_color(char c,realcolor_t back,realcolor_t fore){
    //type transform realcolor_t to uint8_t
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;
    //char config
    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute = attribute_byte << 8;
    //check char
    //0x08 is backtrace
    //0x09 is tab,tab means 8 blanks,and cursor must be aligned in 8 letters(unix)
    if(c == 0x08 && cursor_x){
        cursor_x--;
    }
    else if(c == 0x09){
        cursor_x = (cursor_x + 8) & ~(8-1);
    }
    else if(c == '\r'){
        cursor_x = 0;
    }
    else if(c == '\n'){
        cursor_x = 0;
        cursor_y++;
    }
    else if(c >= ' '){
        vga_base[cursor_y * 80 + cursor_x] = c | attribute;
        cursor_x++;
    }
    if(cursor_x >= 80){
        cursor_x = 0;
        cursor_y++;
    }
    //if need
    scroll();

    //move hardware cursor
    move_cursor();
}

void console_write(char* c_str){
    while(*c_str){
        console_putc_color(*c_str++,BLACK,WHITE);
    }
}

void console_write_color(char* c_str,realcolor_t back,realcolor_t fore){
    while(*c_str){
        console_putc_color(*c_str++,back,fore);
    }    
}

static void move_cursor(){
    //screen width is 80 bytes
    uint16_t cursor_loc = cursor_y * 80 + cursor_x;

    //register 14,15 means the high and low 8 bits of the cursor loc
    outb(0x3D4,14);//high byte
    outb(0x3D5,cursor_loc >> 8);
    outb(0x3D4,15);//low byte
    outb(0x3D5,cursor_loc & 0xFF);
}

static void scroll(){
    //using blank to fill in the screen
    //white background black font
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    if(cursor_y >= 25){
        //the last 24 lines copy to the lines above
        //the first line vanished
        for(int i=0;i<24*80;i++){
            vga_base[i] = vga_base[i+80];
        }
        //fill the 25th line with blanks
        for(int i=0;i<80;i++){
            vga_base[24*80+i] = blank;
        }
        //update curser_y
        cursor_y = 24;
    }    
}

/*output 0x????????,using A~F,0~9*/
void console_write_hex(uint32_t n,realcolor_t back,realcolor_t fore){
    console_write_color("0x",back,fore);
    uint32_t tmp = n;
    char str[9];
    //end str with '\0'
    str[8] = '\0';
    //32 bits include 8 groups of 4 bits
    for(int i=1;i<=8;i++){
        //get the lowest 4 bits
        uint8_t tmpb = tmp & 0x0F;
        //0~9
        if(tmpb < 10)
            str[8-i] = tmpb + '0';
        //A~F
        else
            str[8-i] = tmpb + 'A' - 10;
        tmp = tmp >> 4;
    }
    console_write_color(str,back,fore);
}

void console_write_dec(uint32_t n,realcolor_t back,realcolor_t fore){
    int cnt = 0;
    char str[32];
    if(n == 0) str[cnt++] = '0';
    while(n > 0){
        int now = n % 10;
        n = n / 10;
        str[cnt++] = now + '0';
    }
    //end str with '\0'
    str[cnt] = '\0';
    //swap first and last
    for(int i=0;i< cnt/2;i++){
        char tt = str[i];
        str[i] = str[cnt-1-i];
        str[cnt-1-i] = tt;
    }
    console_write_color(str,back,fore);    
}

void console_init(){
    console_clear();
}