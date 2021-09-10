#ifndef __PMM_H_
#define __PMM_H_
    #include <defs.h>

    struct E820map_fmt{
        uint32_t addr1;
        uint32_t addr2;
        uint32_t len1;
        uint32_t len2;
        uint32_t type;
    };

    struct E820{
        unsigned long addr;
        unsigned long len;
        uint32_t type;
    }__attribute__((packed));

    struct Global_Mem_Desc{
        struct E820 e820[32];
        unsigned long e820_len;
    };

    extern struct Global_Mem_Desc mm_struct;
    
    //8 bytes 
    #define NUMPTE 512 //pte number in 1 page
    #define PAGE_OFFFSET ((unsigned long)0xffff800000000000)

    #define PAGE_GDT_SHIFT 39
    #define PAGE_1G_SHIFT 30
    #define PAGE_2M_SHIFT 21
    #define PAGE_4K_SHIFT 12

    #define PAGE_2M_SIZE (1UL << PAGE_2M_SHIFT)
    #define PAGE_4KK_SIZE (1UL << PAGE_4K_SHIFT)

    #define PAGE_2M_MASK (~(PAGE_2M_SIZE - 1))
    #define PAGE_4K_MASK (~(PAGE_4K_SIZE - 1))

    #define PAGE_2M_ALIGN(addr) (((unsigned long)(addr) + PAGE_2M_SIZE -1) & PAGE_2M_MASK)
    #define PAGE_4K_ALIGN(addr) (((unsigned long)(addr) + PAGE_4K_SIZE -1) & PAGE_4K_MASK)

    #define V2P(addr) ((unsigned long)(addr) - PAGE_OFFSET)
    #define P2V(addr) ((unsigned long)(addr) + PAGE_OFFSET)

    void probe_memory();

#endif