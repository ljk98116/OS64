#include <intr.h>
#include <asmx64.h>
#include <tstdio.h>

intr_handler_t intr_handler[24];

void intr_init(){
    //8259a master
    outb(0x20,0x11);
    outb(0x21,0x20);
    outb(0x21,0x04);
    outb(0x21,0x01);
    //8259a slave
    outb(0xa0,0x11);
    outb(0xa1,0x28);
    outb(0xa1,0x02);
    outb(0xa1,0x01);
    //8259a M/S
    outb(0x21,0xfd);
    outb(0xa1,0xff);

    sti();
}

void do_IRQ(uint64_t regs,uint64_t id){
    uint8_t x;
    x = inb(0x60);
    printk_color(BLACK,RED,"do IRQ:%#08x\t",id);
    printk_color(BLACK,RED,"key_code:%#08x\n",x);
    outb(0x20,0x20);
}