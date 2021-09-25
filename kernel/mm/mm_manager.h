#ifndef __MM_MANAGER_H_
#define __MM_MANAGER_H_
    #include <pmm.h>
    
    struct PMM_Manager{
        char* pmm_manager_name;
        void(*mm_init)();
        struct Page* (*alloc_pages)(int zone_sel,int number,uint64_t flags);
        void (*free_pages)(struct Page *page,int number);
    };

#endif