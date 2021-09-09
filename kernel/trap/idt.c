#include <idt.h>
#include <tstdio.h>
#include <trap.h>

extern uint64_t IDT_Table[];
extern void idt_flush();

idt_entry_lo idt_lo[256];
idt_entry_hi idt_hi[256];

static void set_idt_gate(uint8_t num,uint64_t base,uint8_t flags,uint8_t IST){
    idt_lo[num].base_lo = base & 0xffff;
    idt_lo[num].sel = 0x0008;
    idt_lo[num].flags = flags;
    idt_lo[num].IST_flag = IST;
    idt_lo[num].base_mi = (base >> 16) & 0xffff;

    idt_hi[num].always0 = 0 & 0xffffffff;
    idt_hi[num].base_hi = (base >> 32) & 0xffffffff;

    IDT_Table[num << 1] = *(uint64_t*)&idt_lo[num];
    IDT_Table[(num << 1)+1] = *(uint64_t*)&idt_hi[num];
}

static void set_intr_gate(uint8_t num,uint64_t base,uint8_t IST){
    set_idt_gate(num,base,0x8E,IST);
}

static void set_trap_gate(uint8_t num,uint64_t base,uint8_t IST){
    set_idt_gate(num,base,0x8F,IST);
}

static void set_system_gate(uint8_t num,uint64_t base,uint8_t IST){
    set_idt_gate(num,base,0xEF,IST);
}

static void set_system_intr_gate(uint8_t num,uint64_t base,uint8_t IST){
    set_idt_gate(num,base,0xEE,IST);
}

static void sys_vec_init(){
    set_trap_gate(0,(uint64_t)divide_error,1);
    set_trap_gate(1,(uint64_t)debug_error,1);
    set_intr_gate(2,(uint64_t)nmi,1);
    set_system_gate(3,(uint64_t)int3,1);
    set_system_gate(4,(uint64_t)overflow,1);
    set_system_gate(5,(uint64_t)bounds,1);
    set_trap_gate(6,(uint64_t)undefined_opcode,1);
    set_trap_gate(7,(uint64_t)dev_not_available,1);
    set_trap_gate(8,(uint64_t)double_fault,1);
    set_trap_gate(9,(uint64_t)coprocessor_segment_overrun,1);
    set_trap_gate(10,(uint64_t)invalid_TSS,1);
    set_trap_gate(11,(uint64_t)segment_not_present,1);
    set_trap_gate(12,(uint64_t)stack_segment_fault,1);
    set_trap_gate(13,(uint64_t)general_protection,1);
    set_trap_gate(14,(uint64_t)page_fault,1);
    set_trap_gate(16,(uint64_t)x87_FPU_error,1);
    set_trap_gate(17,(uint64_t)alignment_check,1);
    set_trap_gate(18,(uint64_t)machine_check,1);
    set_trap_gate(19,(uint64_t)SIMD_exception,1);
    set_trap_gate(20,(uint64_t)virtualization_exception,1);
}

void init_idt(){
    for(int i=0;i<256;i++){
        set_idt_gate(i,(uint64_t)unhandled_int,0x8E,0x00);
    }
    sys_vec_init();
    idt_flush();
    printk("IDT64 has sccesfully initialized\n");
}

