#include <pmm.h>
#include <tstdio.h>

struct PMM_Manager pmm_manager;

//global cr3 pointer
uint64_t* global_cr3 = 0;

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

static inline uint64_t* GetCR3(){
    uint64_t *tmp;
    asm volatile("movq %%cr3,%0\n"
                :"=r"(tmp)::"memory");
    return tmp;
}

#define TLB_flush(cr3)     \
do                      \
{                       \                    \
    asm volatile(           \
        "movq %0,%%cr3\n\t" \
        :                   \
        :"r"(cr3)        \
        :"memory"           \
    );                      \
} while (0)             \


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
        printk("start:%#018lx,len:%#018lx,end:%#018lx\n",start,mm_struct.e820[i].len,end);
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
    for(int i=0;i<page_num;i++,p++){
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

static uint64_t set_page_attr(struct Page *page,uint64_t flags){
    if(page == NULL){
        printk("page is NULL\n");
        return 0;
    }
    page->page_attr = flags;
    return 1;
}

static uint64_t get_page_attr(struct Page *page){
    if(page == NULL){
        printk("page is NULL\n");
        return 0;
    }
    return page->page_attr;
}

static unsigned long pgitem_init(struct Page *page,unsigned long flags){
    page->page_attr |= flags;
    if(!page->page_ref || (page->page_attr & PG_Shared)){
        page->page_ref++;
        page->zone_struct->total_page_ref++;
    }
    return 1;
}

static void init_gmd(){
    probe_memory();
    init_mem_bitsmap();
    init_page();
    init_zone();
    for(int i=0;i<=mm_struct.e820_len;i++){
        uint64_t start,end;
        struct Zone *z;
        struct Page *p;
        if(mm_struct.e820[i].type != 1) continue;
        start = PAGE_2M_ALIGN(mm_struct.e820[i].addr);
        end = ((mm_struct.e820[i].addr + mm_struct.e820[i].len) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
        printk("start:%#018lx,len:%#018lx,end:%#018lx\n",start,mm_struct.e820[i].len,end);
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
    set_page_attr(mm_struct.pages_struct,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel);
    //get zone total size
    mm_struct.zones_size = ((mm_struct.zones_len * sizeof(struct Zone) + sizeof(long) -1) & (~(sizeof(long) - 1)));
    //print current gmd struct
    print_gmd();
    //INDEX for ZONES
    ZONE_DMA_INDEX = 0;
    ZONE_NORMAL_INDEX = 0;
    ZONE_UNMAPPED_INDEX = 0;
    printk("Zone_len:%d\n",mm_struct.zones_len);
    for(int i=0;i<mm_struct.zones_len;i++){
        struct Zone *z = mm_struct.zones_struct + i;
        printk_color(BLACK,ORANGE,"zone_start_addr:%#018lx,zone_end_addr:%#018lx,pages_group:%#018lx,pages_len:%#018lx\n",
        z->zone_start_addr,z->zone_end_addr,z->page_struct,z->page_num
        );
        //0x100000000~ is unmapped
        if(z->zone_start_addr >= 0x100000000 && !ZONE_UNMAPPED_INDEX)
            ZONE_UNMAPPED_INDEX = i;
    }
    printk_color(BLACK,ORANGE,"ZONE_DMA_INDEX:%d,ZONE_NORMAL_INDEX:%d,ZONE_UNMAPED_INDEX:%d\n",
    ZONE_DMA_INDEX,ZONE_NORMAL_INDEX,ZONE_UNMAPPED_INDEX);

    //use a blank to seperate mm_struct
    mm_struct.end_of_gmd = (uint64_t)((uint64_t)mm_struct.zones_struct + mm_struct.zones_size + sizeof(long)*32) & (~(sizeof(long) -1));
    //print kernel info
    printk_color(BLACK,ORANGE,"start_code:%#018lx,end_code:%#018lx,end_data:%#018lx,end_brk:%#018lx,end_of_struct:%#018lx\n",
    mm_struct.start_kcode,mm_struct.end_kcode,mm_struct.end_kdata,mm_struct.end_kbrk,mm_struct.end_of_gmd);
    //init pages
    uint64_t pages_total = V2P(mm_struct.end_of_gmd) >> PAGE_2M_SHIFT;
    //printk("end_of_gmd:%#018lx,pages_total:%#018lx\n",V2P(mm_struct.end_of_gmd),pages_total);
    for(uint64_t i=0;i<= pages_total;i++){
        struct Page *tmp = mm_struct.pages_struct + i;
        pgitem_init(tmp,PG_PTable_Maped | PG_Kernel_Init | PG_Kernel | PG_Kernel2);
        *(mm_struct.bitsmap + (tmp->phy_addr >> PAGE_2M_SHIFT >> 6)) |= 1UL << (tmp->phy_addr >> PAGE_2M_SHIFT) % 64;
        tmp->zone_struct->page_use_num++;
        tmp->zone_struct->page_free_num--;
    }

    //cr3
    global_cr3 = GetCR3();
    printk_color(BLACK,INDIGO,"Global CR3:%#018lx\n",global_cr3);
    printk_color(BLACK,INDIGO,"*Global CR3:%#018lx\n",*(uint64_t*)P2V((uint64_t)global_cr3) & (~0xff));
    printk_color(BLACK,INDIGO,"**Global CR3:%#018lx\n",*(uint64_t*)P2V(*(uint64_t*)P2V((uint64_t)global_cr3) & (~0xff)) & (~0xff));
    for(int i=0;i<10;i++){
        *(uint64_t*)P2V((uint64_t)global_cr3 + i) = 0UL;
    }
    //TLB_flush(global_cr3);
}

struct Page* alloc_pages(int zone_sel,int number,uint64_t flags){
    int start_idx = 0;
    int end_idx = 0;
    if(number >= 64 || number <= 0){
        printk("alloc pages error\n");
        return NULL;
    }
    switch(zone_sel){
        case ZONE_DMA:
            start_idx = 0;
            end_idx = ZONE_DMA_INDEX;
            break;
        case ZONE_NORMAL:
            start_idx = ZONE_DMA_INDEX;
            end_idx = ZONE_NORMAL_INDEX;
            break;
        case ZONE_UNMAPPED:
            start_idx = ZONE_UNMAPPED_INDEX;
            end_idx = mm_struct.zones_len - 1;
            break;
        default:
            printk("Not Present\n");
            break;
    }

    //printk("%#018lx,%#018lx\n",start_idx,end_idx);
    for(uint64_t i=start_idx;i<=end_idx;i++){
        struct Zone* z;
        if((mm_struct.zones_struct +i)->page_free_num < number) continue;

        z = mm_struct.zones_struct + i;

        uint64_t page_start_idx,page_end_idx,page_num;
        page_start_idx = z->zone_start_addr >> PAGE_2M_SHIFT;
        page_end_idx = z->zone_end_addr >> PAGE_2M_SHIFT;
        page_num = z->zone_len >> PAGE_2M_SHIFT;

        //bitsmap 64 bits align
        uint64_t tmp = 64 - page_start_idx % 64;
        for(uint64_t j=page_start_idx;j<page_end_idx;j += j % 64 ? tmp:64){
            uint64_t *p = mm_struct.bitsmap + (j >> 6);
            uint64_t shift = j % 64;
            for(uint64_t k=shift ; k< 64-shift;k++){
                uint64_t current64_bitmap = (*p >> k) | (*(p+1) << (64-k));
                if(!( (k? current64_bitmap : *p) & ((1UL << number) - 1) )){
                    uint64_t page = j+k-shift;
                    for(uint64_t l=0;l<number;l++){
                        struct Page *x = mm_struct.pages_struct + page + l;
                        *(mm_struct.bitsmap + (x->phy_addr >> PAGE_2M_SHIFT >> 6)) |= 1UL << (x->phy_addr >> PAGE_2M_SHIFT) % 64;
                        z->page_use_num++;
                        z->page_free_num--;
                        x->page_attr = flags;
                    }
                    return (struct Page*)(mm_struct.pages_struct + page);
                }
            }
        }
    }
    return NULL;
}

void init_pmm(){
    printk("current pmm manager:%s\n",pmm_manager.pmm_manager_name);
    pmm_manager.mm_init();
}

void free_pages(struct Page *page,int number){
    int i = 0;
    if(page == NULL){
        printk("free_page error:page is not valid\n");
        return;
    }
    else if(number >= 64 || number <= 0){
        printk("free_page error:number is not valid\n");
        return;
    }
    for(i=0;i<number;i++,page++){
        *(mm_struct.bitsmap + (page->phy_addr >> PAGE_2M_SHIFT >> 6)) &= ~(1UL << (page->phy_addr >> PAGE_2M_SHIFT) % 64);
    }
}

#ifdef DEFAULT_PMM
    struct PMM_Manager pmm_manager = {
        .pmm_manager_name = "default_pmm_manager",
        .mm_init = init_gmd,
        .alloc_pages = alloc_pages,
        .free_pages = free_pages
    };
#endif