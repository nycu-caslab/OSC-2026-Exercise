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

int hextoi(const char* s, int n) {
    int r = 0;
    while (n-- > 0) {
        r = r << 4;
        if (*s >= 'A')
            r += *s++ - 'A' + 10;
        else if (*s >= 0)
            r += *s++ - '0';
    }
    return r;
}

int align(int n, int byte) {
    return (n + byte - 1) & ~(byte - 1);
}

int memcmp(const void* s1, const void* s2, int n) {
    const unsigned char *a = s1, *b = s2;
    while (n-- > 0) {
        if (*a != *b)
            return *a - *b;
        a++;
        b++;
    }
    return 0;
}

void* memcpy(void* dst, const void* src, int n) {
    char* d = dst;
    const char* s = src;
    while (n--)
        *d++ = *s++;
    return dst;
}

void* memset(void* s, int c, int n) {
    unsigned char* p = s;
    while (n--)
        *p++ = (unsigned char)c;
    return s;
}
