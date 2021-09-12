#include <trap.h>
#include <idt.h>
#include <tstdio.h>

//trap handles
void do_divide_error(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_divide_error(0),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_debug_error(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_debug_error(1),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_nmi(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_nmi(2),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_int3(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_int3(3),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_overflow(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_overflow(4),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_bounds(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_bounds(5),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_undefined_opcode(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_undefined_opcode(6),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_dev_not_available(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_dev_not_available(7),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_double_fault(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_double_fault(8),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_coprocessor_segment_overrun(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_coprocessor_segment_overrun(9),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n",error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_invalid_TSS(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_invalid_TSS(10),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));

    if (error_code & 0X01)
        printk_color(BLACK, RED, "The exception occurred during delievery of an event external to the program,such as an interrupt or an earlier exception\n");
    if (error_code & 0x02)
        printk_color(BLACK, RED, "Refers to a gate descriptor in the IDT;\n");
    else
        printk_color(BLACK, RED, "Refers to a descriptor in the GDT or the current LDT;\n");

    if ((error_code & 0x02) == 0)
    {
        if (error_code & 0x04)
            printk_color(BLACK, RED, "Refers to a segment or gate descriptor in the LDT;\n");
        else
            printk_color(BLACK, RED, "Refers to a descriptor in the current GDT;\n");
    }
    while (1)
        ;
}

void do_segment_not_present(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_segment_not_present(11),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_stack_segment_fault(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_stack_segment_fault(12),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_general_protection(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_general_protection(13),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_page_fault(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_page_fault(14),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));

    uint64_t cr2 = 0;
    asm volatile("movq %%cr2,%0"
                 : "=r"(cr2)::"memory");

    if (!(error_code & 0x01))
        printk_color(BLACK, RED, "Page Not Present,\t");
    if (error_code & 0x02)
        printk_color(BLACK, RED, "Write Cause Fault,\t");
    else
        printk_color(BLACK, RED, "Read Cause Fault,\t");

    if (error_code & 0x04)
        printk_color(BLACK, RED, "Fault in user(3)\t");
    else
        printk_color(BLACK, RED, "Fault in supervisor(0,1,2)\t");

    if (error_code & 0x08)
        printk_color(BLACK, RED, "Reserved Bit Cause Fault\t");
    if (error_code & 0x10)
        printk_color(BLACK, RED, "Instruction fetch Cause Fault");

    printk_color(BLACK, RED, "\n");
    printk_color(BLACK, RED, "CR2:0x%08x%08x\n", cr2);
    while (1)
        ;
}

void do_x87_FPU_error(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_x87_FPU_err(16),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_alignment_check(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_alignment_check(17),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_machine_check(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_machine_check(18),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_SIMD_exception(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_SIMD_exception(19),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void do_virtualization_exception(uint64_t rsp, uint64_t error_code)
{
    uint64_t* p = NULL;
    p = (uint64_t* )(rsp + 0x98);
    printk_color(BLACK, RED, "do_virtualization_exception(20),ERR_CODE:0x%08x%08x,RSP:0x%08x%08x,RIP:0x%08x%08x\n", error_code >> 32,error_code, rsp >> 32,rsp,(*p+0xffff800000000000) >> 32, (*p+0xffff800000000000));
    while (1)
        ;
}

void init_alltrap()
{
    init_idt();
}