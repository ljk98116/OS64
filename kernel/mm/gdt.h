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

    void SetTSS64(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,
    unsigned long ist1,unsigned long ist2,unsigned long ist3,unsigned long ist4,
    unsigned long ist5,unsigned long ist6,unsigned long ist7);
    void init_gdt();
#endif