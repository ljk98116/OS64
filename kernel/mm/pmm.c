#include <pmm.h>
#include <tstdio.h>

void probe_memory(){
    unsigned long TotalMem = 0;
    struct E820 *p = NULL;
    printk_color(BLACK,BLUE,"Display Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others Undefine)\n");
    p = (struct E820 *)0xffff800000007e00;
    for(int i=0;i<32;i++){
        printk_color(
            BLACK,ORANGE,
            "Address:0x%08x 0x%08x\tLength:0x%08x 0x%08x\tType:0x%08x\n",
            p->addr >> 32,p->addr,p->len >> 32,p->len,p->type
        );
        if(p->type == 1){
            TotalMem += p->len;
        }
        mm_struct.e820[i].addr = p->addr;
        mm_struct.e820[i].len = p->len;
        mm_struct.e820[i].type = p->type;
        mm_struct.e820_len = i;
        p++;
        if(p->type > 4) break;
    }
    printk_color(BLACK,ORANGE,"Total Mem:0x%08x%08x\n",TotalMem >> 32,TotalMem);

    TotalMem = 0;
    for(int i=0;i<= mm_struct.e820_len;i++){
        unsigned long start,end;
        if(mm_struct.e820[i].type != 1) continue;
        start = PAGE_2M_ALIGN(mm_struct.e820[i].addr);
        end = ((mm_struct.e820[i].addr + mm_struct.e820[i].len) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
        if(end <= start) continue;
        TotalMem += (end-start) >> PAGE_2M_SHIFT;
    }
    printk_color(BLACK,ORANGE,"TotalMem Free 2M Page Number:%010x=%010d\n",TotalMem,TotalMem);
}