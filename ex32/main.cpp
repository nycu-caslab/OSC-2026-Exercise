#include <iostream>
#include <list>
#include <vector>

#define PAGE_SIZE (1UL << 12)
#define NUM_PAGES 0x280000
#define MAX_ORDER 10

typedef unsigned long phys_addr_t;

struct page {
    int order = 0;
    int refcount = 0;
};

std::vector<page> mem_map;
std::vector<std::list<page*>> free_area;

struct page* get_buddy(struct page* page, unsigned int order) {
    return &mem_map[(page - mem_map.data()) ^ (1 << order)];
}

void memory_reserve(phys_addr_t base, size_t size) {
    // TODO: Implement this function
}

void dump() {
    for (int i = MAX_ORDER; i >= 0; i--)
        std::cout << "free_area[" << i << "] " << free_area[i].size()
                  << std::endl;
}

void mm_init() {
    mem_map.resize(NUM_PAGES);
    free_area.resize(MAX_ORDER + 1);
    for (size_t i = 0; i < NUM_PAGES; i += (1 << MAX_ORDER)) {
        mem_map[i].order = MAX_ORDER;
        free_area[MAX_ORDER].push_back(&mem_map[i]);
    }
    memory_reserve(0, 0x82a69510);
}

int main() {
    mm_init();
    dump();
    return 0;
}
