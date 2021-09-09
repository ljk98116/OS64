.globl idt_flush
idt_flush:
	lidt IDT_POINTER(%rip)
	retq

.globl tss64_flush
tss64_flush:
	lgdt GDT_POINTER(%rip)
	movw $0x40,%ax
	ltr %ax
	retq

.globl unhandled_int
unhandled_int:
	cld
	pushq	%rax
	pushq	%rbx
	pushq	%rcx
	pushq	%rdx
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi

	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15

    movq %ds,%rax
    pushq %rax

    movq $0x10,%rax
    movq %rax,%ds
    movq %rax,%es
    movq %rax,%fs
    movq %rax,%gs
    movq %rax,%ss

    pushq %rsp
	callq panic_normal
	addq $4,%rsp

Loop:
	popq	%rax
	movq	%rax,	%ds
	movq	%rax,	%es
    movq	%rax,	%fs
    movq	%rax,	%gs
    movq	%rax,	%ss

	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%r11
	popq	%r10
	popq	%r9
	popq	%r8

	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%rdx
	popq	%rcx
	popq	%rbx
	popq	%rax

    addq $8,%rsp
	iretq
