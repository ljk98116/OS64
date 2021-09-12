#ifndef __TRAP_H_
#define __TRAP_H_
    #include <defs.h>
    //trap entry tools

    void init_alltrap();

    //trap handles
    void do_divide_error(uint64_t,uint64_t);
    void do_debug_error(uint64_t,uint64_t);
    void do_nmi(uint64_t,uint64_t);
    void do_int3(uint64_t,uint64_t);
    void do_overflow(uint64_t,uint64_t);
    void do_bounds(uint64_t,uint64_t);
    void do_undefined_opcode(uint64_t,uint64_t);
    void do_dev_not_available(uint64_t,uint64_t);
    void do_double_fault(uint64_t,uint64_t);
    void do_coprocessor_segment_overrun(uint64_t,uint64_t);
    void do_invalid_TSS(uint64_t,uint64_t); 
    void do_segment_not_present(uint64_t,uint64_t);   
    void do_stack_segment_fault(uint64_t,uint64_t);
    void do_general_protection(uint64_t,uint64_t);
    void do_page_fault(uint64_t,uint64_t);
    void do_x87_FPU_error(uint64_t,uint64_t);
    void do_alignment_check(uint64_t,uint64_t);
    void do_machine_check(uint64_t,uint64_t);
    void do_SIMD_exception(uint64_t,uint64_t);
    void do_virtualization_exception(uint64_t,uint64_t);
    
#endif