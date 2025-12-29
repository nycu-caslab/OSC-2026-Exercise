#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cpio_t {
    char magic[6];
    char ino[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char devmajor[8];
    char devminor[8];
    char rdevmajor[8];
    char rdevminor[8];
    char namesize[8];
    char check[8];
};

/**
 * @brief Convert a hexadecimal string to integer
 *
 * @param s hexadecimal string
 * @param n length of the string
 * @return integer value
 */
static int hextoi(const char* s, int n) {
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

/**
 * @brief Align a number to the nearest multiple of a given number
 *
 * @param n number
 * @param byte alignment
 * @return aligned number
 */
static int align(int n, int byte) {
    return (n + byte - 1) & ~(byte - 1);
}

void initrd_list(const void* rd) {
    // TODO: Implement this function
}

void initrd_cat(const void* rd, const char* filename) {
    // TODO: Implement this function
}

int main() {
    /* Prepare the initial RAM disk */
    FILE* fp = fopen("initramfs.cpio", "rb");
    if (!fp) {
        perror("fopen");
        return EXIT_FAILURE;
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    void* rd = malloc(sz);
    fseek(fp, 0, SEEK_SET);
    if (fread(rd, 1, sz, fp) != sz) {
        fprintf(stderr, "Failed to read the device tree blob\n");
        free(rd);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);

    initrd_list(rd);
    initrd_cat(rd, "osc.txt");
    initrd_cat(rd, "test.txt");

    free(rd);
    return 0;
}
