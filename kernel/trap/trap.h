#ifndef __TRAP_H_
#define __TRAP_H_
    #include <defs.h>
    //trap entry tools

    void init_alltrap();

    //trap handles
    void do_divide_error(unsigned long,unsigned long);
    void do_debug_error(unsigned long,unsigned long);
    void do_nmi(unsigned long,unsigned long);
    void do_int3(unsigned long,unsigned long);
    void do_overflow(unsigned long,unsigned long);
    void do_bounds(unsigned long,unsigned long);
    void do_undefined_opcode(unsigned long,unsigned long);
    void do_dev_not_available(unsigned long,unsigned long);
    void do_double_fault(unsigned long,unsigned long);
    void do_coprocessor_segment_overrun(unsigned long,unsigned long);
    void do_invalid_TSS(unsigned long,unsigned long); 
    void do_segment_not_present(unsigned long,unsigned long);   
    void do_stack_segment_fault(unsigned long,unsigned long);
    void do_general_protection(unsigned long,unsigned long);
    void do_page_fault(unsigned long,unsigned long);
    void do_x87_FPU_error(unsigned long,unsigned long);
    void do_alignment_check(unsigned long,unsigned long);
    void do_machine_check(unsigned long,unsigned long);
    void do_SIMD_exception(unsigned long,unsigned long);
    void do_virtualization_exception(unsigned long,unsigned long);
    
#endif