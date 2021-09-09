#include <gdt.h>
#include <tstdio.h>

tss_descriptor_t tss_desc;

extern void tss64_flush();
extern uint64_t GDT_Table[];
extern uint64_t TSS64_Table[];
extern uint16_t TSS64_LIMIT;

static void init_tss_descriptor(){
    uint64_t tss_base = (uint64_t)TSS64_Table;
    tss_desc.lim_lo = TSS64_LIMIT;
    tss_desc.always0 = 0;
    tss_desc.base_lo = tss_base & 0xffff;
    tss_desc.base_mi_1 = (tss_base >> 16) & 0xff;
    tss_desc.flags = 0x89;
    tss_desc.limhi_flag = 0;
    tss_desc.base_mi_2 = (tss_base >> 24) & 0xff;
    tss_desc.base_hi = (tss_base >> 32) & 0xffffffff;
    __uint128_t tss_val = *(__uint128_t*)&tss_desc;
    GDT_Table[8] = tss_val & 0xffffffffffffffff;
    GDT_Table[9] = (tss_val >> 64) & 0xffffffffffffffff;
    tss64_flush();
}

void SetTSS64(
unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,
unsigned long ist1,unsigned long ist2,unsigned long ist3,unsigned long ist4,
unsigned long ist5,unsigned long ist6,unsigned long ist7)
{
    uint32_t *p = (uint32_t*)TSS64_Table;
    *(unsigned long*)(p + 1) = rsp0;
    *(unsigned long*)(p + 3) = rsp1;
    *(unsigned long*)(p + 5) = rsp2;

    *(unsigned long*)(p + 9) = ist1;
    *(unsigned long*)(p + 11) = ist2;
    *(unsigned long*)(p + 13) = ist3;
    *(unsigned long*)(p + 15) = ist4;
    *(unsigned long*)(p + 17) = ist5;
    *(unsigned long*)(p + 19) = ist6;
    *(unsigned long*)(p + 21) = ist7;
}

void init_gdt(){
    SetTSS64(
    0xffff800000007c00,0xffff800000007c00,0xffff800000007c00,
    0xffff800000007c00,0xffff800000007c00,0xffff800000007c00,0xffff800000007c00,
    0xffff800000007c00,0xffff800000007c00,0xffff800000007c00
    );

    init_tss_descriptor();
    printk("GDT64 has succesfully initialized !!\n");
}