#include <stdio.h>

#include "vfs.h"

#define MAX_FD 16

struct file* fdt[MAX_FD] = {0};
extern struct mount* rootfs;
extern struct filesystem fs_list[];

long my_open(const char* pathname, int flags);
long my_close(int fd);
long my_read(int fd, char* buf, long count);
long my_write(int fd, const char* buf, long count);

int main() {
    rootfs = malloc(sizeof(struct mount));
    struct filesystem fs = {.name = "tmpfs", .setup_mount = tmpfs_setup_mount};
    int id = register_filesystem(&fs);
    fs_list[id].setup_mount(&fs_list[id], rootfs);

    int fd = 0;
    int len = 0;
    fd = my_open("/file.txt", O_CREAT);
    len = my_write(fd, "Operating Systems Capstone", 26);
    my_close(fd);

    char buf[64] = {0};
    fd = my_open("/file.txt", 0);
    len = my_read(fd, buf, sizeof(buf) - 1);
    my_close(fd);

    printf((strcmp(buf, "Operating Systems Capstone") == 0)
               ? "Test passed. Nice work!\n"
               : "Test failed. Keep trying!\n");
    return 0;
}

long my_open(const char* pathname, int flags) {
    for (int i = 0; i < MAX_FD; i++) {
        if (fdt[i] == 0 && vfs_open(pathname, flags, &fdt[i]) == 0)
            return i;
    }
    return -1;
}

long my_close(int fd) {
    if (fdt[fd]) {
        vfs_close(fdt[fd]);
        fdt[fd] = 0;
        return 0;
    }
    return -1;
}

long my_read(int fd, char* buf, long count) {
    if (fdt[fd])
        return vfs_read(fdt[fd], buf, count);
    return -1;
}

long my_write(int fd, const char* buf, long count) {
    if (fdt[fd])
        return vfs_write(fdt[fd], buf, count);
    return -1;
}
