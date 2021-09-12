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
uint64_t rsp0,uint64_t rsp1,uint64_t rsp2,
uint64_t ist1,uint64_t ist2,uint64_t ist3,uint64_t ist4,
uint64_t ist5,uint64_t ist6,uint64_t ist7)
{
    uint32_t *p = (uint32_t*)TSS64_Table;
    *(uint64_t*)(p + 1) = rsp0;
    *(uint64_t*)(p + 3) = rsp1;
    *(uint64_t*)(p + 5) = rsp2;

    *(uint64_t*)(p + 9) = ist1;
    *(uint64_t*)(p + 11) = ist2;
    *(uint64_t*)(p + 13) = ist3;
    *(uint64_t*)(p + 15) = ist4;
    *(uint64_t*)(p + 17) = ist5;
    *(uint64_t*)(p + 19) = ist6;
    *(uint64_t*)(p + 21) = ist7;
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