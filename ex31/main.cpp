#include <iostream>
#include <list>
#include <vector>

#define NUM_PAGES 0x280000
#define MAX_ORDER 10

struct page {
    int order = 0;
    int refcount = 0;
};

std::vector<page> mem_map;
std::vector<std::list<page*>> free_area;

struct page* get_buddy(struct page* page, unsigned int order) {
    return &mem_map[(page - mem_map.data()) ^ (1 << order)];
}

struct page* alloc_pages(unsigned int order) {
    // TODO: Implement this function
}

void free_pages(struct page* page) {
    // TODO: Implement this function
}

void dump() {
    for (int i = MAX_ORDER; i >= 0; i--)
        std::cout << "free_area[" << i << "] " << free_area[i].size()
                  << std::endl;
}

int main() {
    mem_map.resize(NUM_PAGES);
    free_area.resize(MAX_ORDER + 1);
    for (size_t i = 0; i < NUM_PAGES; i += (1 << MAX_ORDER)) {
        mem_map[i].order = MAX_ORDER;
        free_area[MAX_ORDER].push_back(&mem_map[i]);
    }

    std::cout << "\np1:\n";
    struct page* p1 = alloc_pages(1);
    dump();

    std::cout << "\np2:\n";
    struct page* p2 = alloc_pages(1);
    dump();

    std::cout << "\np3:\n";
    struct page* p3 = alloc_pages(1);
    dump();

    free_pages(p1);
    free_pages(p2);
    free_pages(p3);

    std::cout << "\nfree:\n";
    dump();
    return 0;
}
