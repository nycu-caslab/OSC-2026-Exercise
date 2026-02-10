#include "vfs.h"

#define MAX_FS   16
#define MAX_FD   16
#define PATH_MAX 255

struct mount* rootfs;
struct filesystem fs_list[MAX_FS];

int register_filesystem(struct filesystem* fs) {
    for (int i = 0; i < MAX_FS; i++) {
        if (fs_list[i].name == NULL) {
            fs_list[i].name = fs->name;
            fs_list[i].setup_mount = fs->setup_mount;
            return i;
        }
    }
    return -1;
}

int vfs_open(const char* pathname, int flags, struct file** target) {
    struct vnode* vnode;
    if (vfs_lookup(pathname, &vnode) != 0 && flags & O_CREAT) {
        int pos = 0;
        for (int i = 0; i < strlen(pathname); i++)
            if (pathname[i] == '/')
                pos = i;
        char dirname[PATH_MAX] = {0};
        strncpy(dirname, pathname, pos);
        const char* filename = (pathname + pos + 1);
        if (vfs_lookup(dirname, &vnode) != 0)
            return -1;
        vnode->v_ops->create(vnode, &vnode, filename);
    }
    (*target) = malloc(sizeof(struct file));
    (*target)->flags = flags;
    vnode->f_ops->open(vnode, target);
    return 0;
}

int vfs_close(struct file* file) {
    return file->f_ops->close(file);
}

int vfs_read(struct file* file, void* buf, size_t len) {
    return file->f_ops->read(file, buf, len);
}

int vfs_write(struct file* file, const void* buf, size_t len) {
    return file->f_ops->write(file, buf, len);
}

int vfs_lookup(const char* pathname, struct vnode** target) {
    if (strlen(pathname) == 0) {
        *target = rootfs->root;
        return 0;
    }

    struct vnode* node = rootfs->root;
    char component[PATH_MAX] = {0};
    int idx = 0;

    for (int i = 1; i < strlen(pathname); i++) {
        if (pathname[i] == '/') {
            component[idx] = '\0';
            if (node->v_ops->lookup(node, &node, component) != 0)
                return -1;
            while (node->mount)
                node = node->mount->root;
            idx = 0;
        } else {
            component[idx++] = pathname[i];
        }
    }
    component[idx] = '\0';

    if (node->v_ops->lookup(node, &node, component) != 0)
        return -1;

    while (node->mount)
        node = node->mount->root;

    *target = node;
    return 0;
}
