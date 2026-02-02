extern char _end[];
static unsigned long curr = (unsigned long)&_end;

void* kmalloc(unsigned long size) {
    curr = (curr + 0xf) & ~0xf;
    void* p = (void*)curr;
    curr += size;
    return p;
}

void* alloc_page() {
    curr = (curr + 0x1000 - 1) & ~(0x1000 - 1);
    void* p = (void*)curr;
    curr += 0x1000;
    return p;
}
