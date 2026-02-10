#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define O_CREAT 00000100

struct vnode {
    struct mount* mount;
    struct vnode_operations* v_ops;
    struct file_operations* f_ops;
    void* internal;
};

struct file {
    struct vnode* vnode;
    size_t f_pos;
    struct file_operations* f_ops;
    int flags;
};

struct mount {
    struct vnode* root;
    struct filesystem* fs;
};

struct filesystem {
    const char* name;
    int (*setup_mount)(struct filesystem* fs, struct mount* mount);
};

struct file_operations {
    int (*open)(struct vnode* file_node, struct file** target);
    int (*close)(struct file* file);
    int (*read)(struct file* file, void* buf, size_t len);
    int (*write)(struct file* file, const void* buf, size_t len);
};

struct vnode_operations {
    int (*lookup)(struct vnode* dir_node,
                  struct vnode** target,
                  const char* component_name);
    int (*create)(struct vnode* dir_node,
                  struct vnode** target,
                  const char* component_name);
};

int register_filesystem(struct filesystem* fs);
int vfs_open(const char* pathname, int flags, struct file** target);
int vfs_close(struct file* file);
int vfs_read(struct file* file, void* buf, size_t len);
int vfs_write(struct file* file, const void* buf, size_t len);
int vfs_lookup(const char* pathname, struct vnode** target);

int tmpfs_setup_mount(struct filesystem* fs, struct mount* mnt);
int tmpfs_open(struct vnode* file_node, struct file** target);
int tmpfs_close(struct file* file);
int tmpfs_read(struct file* file, void* buf, size_t len);
int tmpfs_write(struct file* file, const void* buf, size_t len);
int tmpfs_lookup(struct vnode* dir_node,
                 struct vnode** target,
                 const char* component_name);
int tmpfs_create(struct vnode* dir_node,
                 struct vnode** target,
                 const char* component_name);
