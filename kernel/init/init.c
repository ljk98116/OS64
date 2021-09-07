#include <console.h>
#include <tstdio.h>

static void TestPrintFrame();

void Start_Kernel(){
    TestPrintFrame();
    console_init();
    console_clear();
    printk("hello os kernel %d\n",1);
    printk_color(BLACK,BLUE,"hello os kernel %d\n",2);
    while(1);
}

static void TestPrintFrame(){
   int *addr = (int*)0xffff800000a00000;
    for(int i=0;i<1440*20;i++){
        *((char*)addr) = (char)0x00;
        *((char*)addr + 1) = (char)0x00;
        *((char*)addr + 2) = (char)0xff;
        *((char*)addr + 3) = (char)0x00;
        addr += 1;
    }
    for(int i=0;i<1440*20;i++){
        *((char*)addr) = (char)0x00;
        *((char*)addr + 1) = (char)0xff;
        *((char*)addr + 2) = (char)0x00;
        *((char*)addr + 3) = (char)0x00;
        addr++;
    }
    for(int i=0;i<1440*20;i++){
        *((char*)addr) = (char)0xff;
        *((char*)addr + 1) = (char)0x00;
        *((char*)addr + 2) = (char)0x00;
        *((char*)addr + 3) = (char)0x00;
        addr++;
    }
    for(int i=0;i<1440*20;i++){
        *((char*)addr) = (char)0xff;
        *((char*)addr + 1) = (char)0xff;
        *((char*)addr + 2) = (char)0xff;
        *((char*)addr + 3) = (char)0x00;
        addr++;
    }
}