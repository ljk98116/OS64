#ifndef __PMM_H_
#define __PMM_H_
    #include <defs.h>
    #include <mm_manager.h>

    //config
    #define DEFAULT_PMM

    struct PMM_Manager pmm_manager;

    struct E820{
        uint64_t addr;
        uint64_t len;
        uint32_t type;
    }__attribute__((packed));

    struct Global_Mem_Desc{
        //e820 
        struct E820 e820[32];
        uint64_t e820_len;

        //memory bitmap
        uint64_t *bitsmap;
        uint64_t bits_size;
        uint64_t bits_len;

        struct Page *pages_struct;
        uint64_t pages_size;
        uint64_t pages_len;

        struct Zone *zones_struct;
        uint64_t zones_size;
        uint64_t zones_len;

        //kseg start and end addrs,kernel end addr
        uint64_t start_kcode,end_kcode;
        uint64_t end_kdata,end_kbrk;

        //end addr of this struct
        uint64_t end_of_gmd;
    };

    //8 bytes 
    #define NUMPTE 512 //pte number in 1 page
    #define PAGE_OFFSET ((uint64_t)0xffff800000000000)

    #define PAGE_GDT_SHIFT 39
    #define PAGE_1G_SHIFT 30
    #define PAGE_2M_SHIFT 21
    #define PAGE_4K_SHIFT 12

    #define PAGE_2M_SIZE (1UL << PAGE_2M_SHIFT)
    #define PAGE_4K_SIZE (1UL << PAGE_4K_SHIFT)

    #define PAGE_2M_MASK (~(PAGE_2M_SIZE - 1))
    #define PAGE_4K_MASK (~(PAGE_4K_SIZE - 1))

    #define PAGE_2M_ALIGN(addr) (((uint64_t)(addr) + PAGE_2M_SIZE -1) & PAGE_2M_MASK)
    #define PAGE_4K_ALIGN(addr) (((uint64_t)(addr) + PAGE_4K_SIZE -1) & PAGE_4K_MASK)

    #define V2P(addr) ((addr) - (PAGE_OFFSET))
    #define P2V(addr) ((addr) + (PAGE_OFFSET))

    //page struct
    struct Page{
        struct Zone *zone_struct;
        uint64_t phy_addr;
        uint64_t page_attr;
        uint64_t page_ref;
        uint64_t page_age;
    };

    //zone struct
    struct Zone{
        struct Page *page_struct;
        uint64_t page_num;

        uint64_t zone_start_addr;
        uint64_t zone_end_addr;
        uint64_t zone_len;
        uint64_t zone_attr;

        struct Global_Mem_Desc *gmd_struct;

        uint64_t page_use_num;
        uint64_t page_free_num;

        uint64_t total_page_ref;
    };

    //Zone attrs
    #define ZONE_DMA  (1<<0)
    #define ZONE_NORMAL (1 << 1)
    #define ZONE_UNMAPPED (1 << 2)

    //PAGE attrs
    #define PG_PTable_Maped	(1 << 0)
    #define PG_Kernel_Init	(1 << 1)
    #define PG_Referenced	(1 << 2)
    #define PG_Dirty	(1 << 3)
    #define PG_Active	(1 << 4)
    #define PG_Up_To_Date	(1 << 5)
    #define PG_Device	(1 << 6)
    #define PG_Kernel	(1 << 7)
    #define PG_K_Share_To_U	(1 << 8)
    #define PG_Slab		(1 << 9)

    //global cr3
    extern uint64_t* global_cr3;

    void init_pmm();
#endif