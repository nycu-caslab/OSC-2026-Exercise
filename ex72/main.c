#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH      4096
#define MAX_COMPONENT 256

const char* curr_working_dir = "/path/to/current/directory";

/**
 * Resolve a relative or absolute filepath
 */
char* my_realpath(const char* path, char* resolved_path) {
    // TODO: Implement this function
}

int main() {
    char resolved[MAX_PATH];
    const char* test_paths[] = {
        ".",
        "..",
        "./test",
        "../parent",
        "dir1/dir2/../../dir3",
        "/absolute/path",
        "relative/./path",
        NULL,
    };
    for (int i = 0; test_paths[i] != NULL; i++) {
        printf("[%d] \"%s\"", i, test_paths[i]);
        if (my_realpath(test_paths[i], resolved))
            printf(" --> \"%s\"\n", resolved);
    }
    return 0;
}
