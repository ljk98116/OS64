#include <pmm.h>
#include <tstdio.h>

//external GMD struct
extern struct Global_Mem_Desc mm_struct;

//external segments
extern uint8_t _text,_etext,_edata,_end;

//TotalMem Valid
static uint64_t TotalMem_Valid = 0;

//Zone Indexes
static int ZONE_DMA_INDEX = 0;
static int ZONE_NORMAL_INDEX = 0;
static int ZONE_UNMAPPED_INDEX = 0;

static void probe_memory(){
    uint64_t TotalMem = 0;
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
        mm_struct.e820[i].addr += p->addr;
        mm_struct.e820[i].len += p->len;
        mm_struct.e820[i].type = p->type;
        mm_struct.e820_len = i;
        p++;
        if(p->type > 4 || p->len == 0 || p->type < 1) break;
    }
    printk_color(BLACK,ORANGE,"Total Mem:0x%08x%08x\n",TotalMem >> 32,TotalMem);

    TotalMem = 0;
    for(int i=0;i<= mm_struct.e820_len;i++){
        uint64_t start,end;
        if(mm_struct.e820[i].type != 1) continue;
        start = PAGE_2M_ALIGN(mm_struct.e820[i].addr);
        end = ((mm_struct.e820[i].addr + mm_struct.e820[i].len) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
        if(end <= start) continue;
        TotalMem += (end-start) >> PAGE_2M_SHIFT;
    }
    printk_color(BLACK,ORANGE,"TotalMem Free 2M Page Number:%010x=%010d\n",TotalMem,TotalMem);
    TotalMem_Valid = mm_struct.e820[mm_struct.e820_len].addr + mm_struct.e820[mm_struct.e820_len].len;
}

static void init_mem_bitsmap(){
    mm_struct.start_kcode = (uint64_t)& _text;
    mm_struct.end_kcode = (uint64_t)& _etext;
    mm_struct.end_kdata = (uint64_t)& _edata;
    mm_struct.end_kbrk = (uint64_t)& _end;

    mm_struct.bitsmap = (uint64_t*)((mm_struct.end_kbrk +PAGE_4K_SIZE-1) & PAGE_4K_MASK);
    //phy page num
    mm_struct.bits_len = TotalMem_Valid >> PAGE_2M_SHIFT;
    //bitmap size in mem,count by bytes
    mm_struct.bits_size = (((uint64_t)(TotalMem_Valid >> PAGE_2M_SHIFT) + sizeof(long) * 8 -1) / 8) & (~(sizeof(long)-1));
    //count by bytes,set all pages are using
    memset(mm_struct.bitsmap,0xff,mm_struct.bits_size);
}

static void init_page(){
    mm_struct.pages_struct = (struct Page*)( ((uint64_t)mm_struct.bitsmap + mm_struct.bits_size + PAGE_4K_SIZE -1) & PAGE_4K_MASK);
    mm_struct.pages_size = ((TotalMem_Valid >> PAGE_2M_SHIFT) * sizeof(struct Page) + sizeof(long) -1) & (~(sizeof(long) - 1));
    mm_struct.pages_len = TotalMem_Valid >> PAGE_2M_SHIFT;
}

static void init_zone(){
    mm_struct.zones_struct = (struct Zone*)(((uint64_t)mm_struct.pages_struct + mm_struct.pages_size + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);
    mm_struct.zones_len = 0;
    mm_struct.zones_size = 0;
}

static void zone_init(struct Zone **z,uint64_t start,uint64_t end){
    *z = mm_struct.zones_struct + mm_struct.zones_len;

    (*z)->zone_start_addr = start;
    (*z)->zone_end_addr = end;
    (*z)->zone_len = end - start;

    (*z)->page_struct = (struct Page*)(mm_struct.pages_struct + (start >> PAGE_2M_SHIFT));
    (*z)->page_num = (end-start) >> PAGE_2M_SHIFT;
    (*z)->page_use_num = 0;
    (*z)->page_free_num = (end - start) >> PAGE_2M_SHIFT;
    (*z)->total_page_ref = 0;

    (*z)->zone_attr = 0;
    (*z)->gmd_struct = &mm_struct;

    //update mm_struct zones_len
    mm_struct.zones_len ++;
}

static void page_init(struct Zone* z,struct Page *p,uint64_t start,uint64_t end,uint64_t page_num){
    for(int i=0;i<page_num;i++){
        p->zone_struct = z;
        p->page_age = 0;
        p->phy_addr = start + PAGE_2M_SIZE * i;
        p->page_attr = 0;
        p->page_ref = 0;
        //use 64 bits as a unit,avoid overflow
        *(mm_struct.bitsmap + (p->phy_addr >> PAGE_2M_SHIFT >> 6)) ^= 1UL << (p->phy_addr >> PAGE_2M_SHIFT) % 64;
    }
}

static void print_gmd(){
    //bitmaps
    printk_color(BLACK,ORANGE,"bitsmap:0x%08x%08x,bits_size:0x%08x%08x,bits_len:0x%08x%08x\n",
    (uint64_t)mm_struct.bitsmap >> 32,(uint64_t)mm_struct.bitsmap,
    mm_struct.bits_size >> 32,mm_struct.bits_size,
    mm_struct.bits_len >> 32,mm_struct.bits_len);
    //pages
    printk_color(BLACK,ORANGE,"pages_struct:0x%08x%08x,pages_size:0x%08x%08x,pages_len:0x%08x%08x\n",
    (uint64_t)mm_struct.pages_struct >> 32,(uint64_t)mm_struct.pages_struct,
    mm_struct.pages_size >> 32,mm_struct.pages_size,
    mm_struct.pages_len >> 32,mm_struct.pages_len
    );
    printk_color(BLACK,ORANGE,"zones_struct:0x%08x%08x,zones_size:0x%08x%08x,zones_len:0x%08x%08x\n",
    (uint64_t)mm_struct.zones_struct >> 32,(uint64_t)mm_struct.zones_struct,
    mm_struct.zones_size >> 32,mm_struct.zones_size,
    mm_struct.zones_len >> 32,mm_struct.zones_len
    );    
}

static void init_gmd(){
    probe_memory();
    init_mem_bitsmap();
    init_page();
    init_zone();
    for(int i=0;i<mm_struct.e820_len;i++){
        uint64_t start,end;
        struct Zone *z;
        struct Page *p;
        if(mm_struct.e820[i].type != 1) continue;
        start = PAGE_2M_ALIGN(mm_struct.e820[i].addr);
        end = ((mm_struct.e820[i].addr + mm_struct.e820[i].len) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
        if(end <= start) continue;
        zone_init(&z,start,end);
        p = z->page_struct;
        page_init(z,p,start,end,z->page_num);
    }
    //init addr 0 to page struct 0,because mm_struct.e820[0].type != 1
    mm_struct.pages_struct->zone_struct = mm_struct.zones_struct;
    mm_struct.pages_struct->phy_addr = 0UL;
    mm_struct.pages_struct->page_attr = 0;
    mm_struct.pages_struct->page_ref = 0;
    mm_struct.pages_struct->page_age = 0;
    //get zone total size
    mm_struct.zones_size = ((mm_struct.zones_len * sizeof(struct Zone) + sizeof(long) -1) & (~(sizeof(long) - 1)));
    //print current gmd struct
    print_gmd();
    //INDEX for ZONES
    ZONE_DMA_INDEX = 0;
    ZONE_NORMAL_INDEX = 0;
    for(int i=0;i<mm_struct.zones_len;i++){
        struct Zone *z = mm_struct.zones_struct + i;
        printk("%#018lx\n",mm_struct.zones_struct);
        printk_color(BLACK,ORANGE,"zone_start_addr:%#018lx,zone_end_addr:%#018lx,pages_group:%#018lx,pages_len:%#018lx\n",
        z->zone_start_addr,z->zone_end_addr,z->page_struct,z->page_num
        );
        //0x100000000~ is unmapped
        if(z->zone_start_addr == 0x100000000)
            ZONE_UNMAPPED_INDEX = i;
    }
    //use a blank to seperate mm_struct
    mm_struct.end_of_gmd = (uint64_t)((uint64_t)mm_struct.zones_struct + mm_struct.zones_size + sizeof(long)*32) & (~(sizeof(long) -1));
    //printk_color(BLACK,ORANGE,"");
}

void init_pmm(){
    init_gmd();
}