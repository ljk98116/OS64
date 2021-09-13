#include <console.h>
#include <tstdio.h>
#include <debug.h>
#include <tstdio.h>
#include <trap.h>
#include <gdt.h>
#include <pmm.h>

//mm struct
struct Global_Mem_Desc mm_struct = {{0},0};

static void TestPrintFrame();
static void TestAllocPages();

void kern_main(){
    TestPrintFrame();
    console_init();
    console_clear();
    printk("hello os kernel %d\n",1);
    printk_color(BLACK,BLUE,"hello os kernel %d\n",2);
    init_gdt();
    init_alltrap();
    init_pmm();
    TestAllocPages();
    print_cur_status();
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

static void TestAllocPages(){
    struct Page *pp = alloc_pages(ZONE_NORMAL,64,PG_PTable_Maped | PG_Active | PG_Kernel);
    if(pp == NULL) {
        printk("Not Exist\n");
        return;
    }
    for(int i=0;i<64;i+=1){
        printk_color(BLACK,INDIGO,"page%d\tattr:%#018lx\taddr:%#018lx\t",
        i,(pp+i)->page_attr,(pp+i)->phy_addr);
        i+=1;
        printk_color(BLACK,INDIGO,"page%d\tattr:%#018lx\taddr:%#018lx\n",
        i,(pp+i)->page_attr,(pp+i)->phy_addr);
    }
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap));
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap+1));
}