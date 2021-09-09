#ifndef __IDT_H_
#define __IDT_H_
    #include <defs.h>

    typedef struct idt_entry_1{
        uint16_t base_lo;
        uint16_t sel;
        uint8_t IST_flag;
        uint8_t flags;
        uint16_t base_mi;
    }__attribute__((packed)) idt_entry_lo;

    typedef struct idt_entry_2{
        uint32_t base_hi;
        uint32_t always0;
    }__attribute__((packed)) idt_entry_hi;

    //trap entry functions

    //undefine
    extern void unhandled_int();
    //traps
    extern void divide_error();
    extern void debug_error();
    //intr
    extern void nmi();
    //system
    extern void int3();
    extern void overflow();
    extern void bounds();
    //trap
    extern void undefined_opcode();
    extern void dev_not_available();
    extern void double_fault();
    extern void coprocessor_segment_overrun();
    extern void invalid_TSS();
    extern void segment_not_present();
    extern void stack_segment_fault();
    extern void general_protection();
    extern void page_fault();
    //extern void unuse();
    extern void x87_FPU_error();
    extern void alignment_check();
    extern void machine_check();
    extern void SIMD_exception();
    extern void virtualization_exception();

    //idt function
    void init_idt();

#endif