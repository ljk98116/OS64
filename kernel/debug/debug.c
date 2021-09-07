#include <debug.h>

static void print_stack_trace();

void print_cur_status(){
	//round is not only in this function
	static int round = 0;
	uint16_t reg1,reg2,reg3,reg4;
	asm volatile(
		"mov %%cs,%0;"
		"mov %%ds,%1;"
		"mov %%es,%2;"
		"mov %%ss,%3;":
		"=m"(reg1),"=m"(reg2),"=m"(reg3),"=m"(reg4)
	);
	//print running state
	printk("%d: @ring %d\n",round,reg1 & 0x3);
	printk("%d: cs = 0x%x\n",round,reg1);
	printk("%d: ds = 0x%x\n",round,reg2);
	printk("%d: es = 0x%x\n",round,reg3);
	printk("%d: ss = 0x%x\n",round,reg4);
	//update round
	++round;
}

void panic(const char* msg){
    printk("*** thomas is in panic: %s ***\n", msg);
    print_stack_trace();
	//loops
	while(1);
}

static void print_stack_trace(){
}