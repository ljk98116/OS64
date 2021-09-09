#include <console.h>
#include <tstdio.h>
#include <font.h>

//20 pixels equal 1 row
static Pos pointer;

static void init_cursor(){
    pointer.XPos = 0;
    pointer.YPos = 0;
    pointer.XRes = 1440;
    pointer.YRes = 900;
    pointer.XCharSize = 8;
    pointer.YCharSize = 16;
    pointer.FB_addr = (uint32_t*)0xffff800000a00000;
    pointer.FB_len = pointer.XRes * pointer.YRes * 4;
}
//Y X
static uint8_t font[56][180];

//get font information from vga
static uint8_t getchar(int x,int y){
    return font[y][x];
}

static void putchar
(   
    uint32_t *fb,int XSize,
    int x,int y,
    uint32_t back,uint32_t fore,
    uint8_t font
)
{
    uint32_t *addr = NULL;
    uint8_t *fontp = NULL;

    int testval = 0;
    fontp = font_ascii[font];
    for(int i=0;i<16;i++){
        addr = fb + XSize * (y+i) + x;
        testval = 0x100;
        for(int j=0;j<8;j++){
            testval = testval >> 1;
            if(*fontp & testval) *addr = fore;
            else *addr = back;
            addr++;
        }
        fontp++;
    }
}

static void scroll_up(){
    pointer.YPos = pointer.YRes / pointer.YCharSize - 1;
}

static void scroll_down(){
    if(pointer.YPos >= pointer.YRes / pointer.YCharSize){
        int max_line = pointer.YRes / pointer.YCharSize;

        //copy the 1-max_row -1 font pixel to 0-max_row
        for(int i=0;i<(max_line - 1) * pointer.XRes * pointer.YCharSize ;i++){
            *(pointer.FB_addr + i) = *(pointer.FB_addr + i + pointer.XRes * pointer.YCharSize);
        }
        for(int i=0;i< max_line - 1;i++){
            for(int j=0;j<pointer.XRes;j++){
                font[i][j] = font[i+1][j];
            }
        }
        for(int j=0;j<pointer.XRes;j++) font[max_line-1][j] = ' ';
        
        //the last font line fills ' '
        for(int i = 0;i<pointer.XRes / pointer.XCharSize;i++){
            putchar(
                pointer.FB_addr,
                pointer.XRes,
                i * pointer.XCharSize,
                (max_line-1) * pointer.YCharSize,
                BLACK,
                WHITE,
                ' '
            );
        }
        pointer.YPos = pointer.YRes / pointer.YCharSize - 1;
    }
}

void console_init(){ 
    init_cursor();
}

void console_clear(){
    for(int i=0;i<pointer.XRes / pointer.XCharSize;i++){
        for(int j=0;j< pointer.YRes / pointer.YCharSize;j++){
            putchar(
            pointer.FB_addr,
            pointer.XRes,
            i*pointer.XCharSize,
            j*pointer.YCharSize,
            BLACK,
            WHITE,
            ' ');
            font[j][i] = ' ';
        }
    }
}

void console_putc_color(char c, uint32_t back, uint32_t fore)
{
    // 0x08 是退格键的 ASCII 码
    // 0x09 是tab 键的 ASCII 码
    uint32_t line = 0;
    int flag = 0;
    if (c == 0x08 && pointer.XPos) {
        pointer.XPos --;
        if(pointer.XPos < 0){
            pointer.XPos = pointer.XRes / pointer.XCharSize -1;
            pointer.YPos --;
        }
        if(pointer.YPos < 0) scroll_up();
    } 
    else if (c == 0x09) {
        line = ((pointer.XPos + 8) & ~(8-1)) - pointer.XPos;
        while(line--){
            console_putc_color(' ',back,fore);
        }
    } 
    else if (c == '\r') {
        pointer.XPos = 0;
    } 
    else if (c == '\n') {
        pointer.XPos = 0;
        pointer.YPos++;
    } 
    else{
        putchar(
            pointer.FB_addr,
            pointer.XRes,
            pointer.XPos * pointer.XCharSize,
            pointer.YPos * pointer.YCharSize,
            back,
            fore,
            c
        );
        font[pointer.YPos][pointer.XPos] = c;
        pointer.XPos++;
    }

    // 每 80 个字符一行，满80就必须换行了
    if (pointer.XPos >= pointer.XRes / pointer.XCharSize) {
        pointer.XPos = 0;
        pointer.YPos ++;
    }

    // 如果需要的话滚动屏幕显示
    scroll_down();
}

void console_write_color(char* str,uint32_t back,uint32_t fore){
    char *p = str;
    while(*p){
        console_putc_color(*p++,back,fore);
    }
}

void console_write(char* str){
    char *p = str;
    while(*p){
        console_putc_color(*p++,BLACK,WHITE);
    }
}