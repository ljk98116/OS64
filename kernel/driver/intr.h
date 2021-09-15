#ifndef __INTR_H_
#define __INTR_H_
//in the middle of the IRQ assembly code
    #define SAVE_ALL    \
    "cld;\n\t"          \
    "pushq %rax;\n\t"   \
    "pushq %rax;\n\t"   \
    "movq %es,%rax;\n\t"\
    "pushq %rax;\n\t"   \
    "movq %ds,%rax;\n\t"\
    "pushq %rax;\n\t"   \
    "xorq %rax,%rax;\n\t"\
    "pushq %rbp;\n\t"   \
    "pushq %rdi;\n\t"   \
    "pushq %rsi;\n\t"   \
    "pushq %rdx;\n\t"   \
    "pushq %rcx;\n\t"   \
    "pushq %rbx;\n\t"   \
    "pushq %r8;\n\t"    \
    "pushq %r9;\n\t"    \
    "pushq %r10;\n\t"   \
    "pushq %r11;\n\t"   \
    "pushq %r12;\n\t"   \
    "pushq %r13;\n\t"   \
    "pushq %r14;\n\t"   \
    "pushq %r15;\n\t"   \
    "movq $0x10,%rdx;\n\t"\
    "movq %rdx,%ds;\n\t"\
    "movq %rdx,%es;\n\t"

#define SYMBOL_NAME_STR(X) #X
#define IRQ_NAME(id) IRQ##id##_interrupt(void)  
#define IRQ_NAME2(id) IRQ##id##_interrupt

#define Build_IRQ(id)       \
    void IRQ_NAME(id);          \
    asm (           \
        SYMBOL_NAME_STR(IRQ)#id"_interrupt: \n\t"\
        "pushq $0x00    \n\t"                    \
        SAVE_ALL                                 \
        "movq %rsp,%rdi\n\t"                     \
        "leaq ret_from_intr(%rip),%rax \n\t"     \
        "pushq %rax\n\t"                         \
        "movq $"#id",%rsi\n\t"                  \
        "jmp do_IRQ\n\t"                        \
    );                      

    Build_IRQ(0x20)
    Build_IRQ(0x21)
    Build_IRQ(0x22)
    Build_IRQ(0x23)
    Build_IRQ(0x24)
    Build_IRQ(0x25)
    Build_IRQ(0x26)
    Build_IRQ(0x27)
    Build_IRQ(0x28)
    Build_IRQ(0x29);
    Build_IRQ(0x2A);
    Build_IRQ(0x2B);
    Build_IRQ(0x2C);
    Build_IRQ(0x2D);
    Build_IRQ(0x2E);
    Build_IRQ(0x2F);
    Build_IRQ(0x30);
    Build_IRQ(0x31);
    Build_IRQ(0x32);
    Build_IRQ(0x33);
    Build_IRQ(0x34);
    Build_IRQ(0x35);
    Build_IRQ(0x36);
    Build_IRQ(0x37);

    void intr_init();

    typedef void (*intr_handler_t)();

    void register_intr_handler(int,intr_handler_t);

    void do_IRQ(uint64_t,uint64_t);

    void (*intr[24])() = {
        IRQ_NAME2(0x20),
        IRQ_NAME2(0x21),
        IRQ_NAME2(0x22),
        IRQ_NAME2(0x23),
        IRQ_NAME2(0x24),
        IRQ_NAME2(0x25),
        IRQ_NAME2(0x26),
        IRQ_NAME2(0x27),
        IRQ_NAME2(0x28),
        IRQ_NAME2(0x29),
        IRQ_NAME2(0x2A),
        IRQ_NAME2(0x2B),
        IRQ_NAME2(0x2C),
        IRQ_NAME2(0x2D),
        IRQ_NAME2(0x2E),
        IRQ_NAME2(0x2F),
        IRQ_NAME2(0x30),
        IRQ_NAME2(0x31),
        IRQ_NAME2(0x32),
        IRQ_NAME2(0x33),
        IRQ_NAME2(0x34),
        IRQ_NAME2(0x35),
        IRQ_NAME2(0x36),
        IRQ_NAME2(0x37)
    };
    
#endif