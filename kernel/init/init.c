#include <console.h>
#include <tstdio.h>
#include <debug.h>
#include <trap.h>
#include <gdt.h>
#include <pmm.h>

//mm struct
struct Global_Mem_Desc mm_struct = {{0},0};

static void TestPrintFrame();
static void TestAllocPages();

extern struct PMM_Manager pmm_manager;

void kern_main(){
    //TestPrintFrame();
    console_init();
    init_gdt();
    init_alltrap();
    printk("hello os kernel %d\n",1);
    printk_color(BLACK,BLUE,"hello os kernel %d\n",2);
    intr_init();
    //int i = 1 / 0;
    print_cur_status();
    init_pmm();
    TestAllocPages();
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
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap));
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap+1));
    struct Page *pp = pmm_manager.alloc_pages(ZONE_NORMAL,63,PG_PTable_Maped | PG_Kernel2);
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap));
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap+1));
    pmm_manager.free_pages(pp,63);
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap));
    printk("mem bitsmap:%#018lx\n",*(mm_struct.bitsmap+1));    
}