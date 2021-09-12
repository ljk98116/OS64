#ifndef __GDT_H_
#define __GDT_H_
    #include <defs.h>
    //TSS struct
    typedef struct tss_descriptor{
        uint16_t lim_lo;
        uint16_t base_lo;
        uint8_t base_mi_1;
        uint8_t flags;
        uint8_t limhi_flag;
        uint8_t base_mi_2;
        uint32_t base_hi;
        uint32_t always0;
    }__attribute__((packed))tss_descriptor_t;

    void SetTSS64(uint64_t rsp0,uint64_t rsp1,uint64_t rsp2,
    uint64_t ist1,uint64_t ist2,uint64_t ist3,uint64_t ist4,
    uint64_t ist5,uint64_t ist6,uint64_t ist7);
    void init_gdt();
#endif