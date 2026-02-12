# Initial Ramdisk Parsing

## Exercise2 Slide
[OSC2026_lab2_exercise](<https://docs.google.com/presentation/d/1_WRWjumH9A3CYNxX_QN-o0WW1o752pQkBS_ohPEzObk/edit?slide=id.g3bd6f2d5062_0_50#slide=id.g3bd6f2d5062_0_50>)

## Intro.
### Initial Ramdisk (initrd)
- A temporary root file system loaded into memory during the boot process.
- Provides essential files and drivers needed to mount the actual root file system.
- We use a cpio archive to serve as the initrd in our lab. See the manual [here](https://man.freebsd.org/cgi/man.cgi?query=cpio&sektion=5). 

## New ASCII Format Cpio Archive
### Archive Structure
**[ Note ]** Padding: align up to 4 bytes
| Structure | 
| ------ | 
| Header | 
| File Name | 
| Padding | 
| File Data | 
| Padding | 

### Header
```c
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
```
**[ Hint ]** You may use these informations:
- `magic`: String of "070701"
- `filesize`
- `namesize`

### Archive Stream
The end of the archive is indicated by a special record with the pathname `"TRAILER!!!"`
| A             | B             | END           |
| ------        | -----         | -----         |
| Header        | Header        | Header        |
| "File_A"      | "File_B"      | "TRAILER!!!"  |
| Padding       | Padding       | (END)         |
| A’s content   | B’s content   |               | 
| Padding       | Padding       |               | 

## TODO
- `initrd_list`: List files in the initrd.
- `initrd_cat`: Display file contents in the initrd.  
- [Expected Result](https://docs.google.com/presentation/d/1_WRWjumH9A3CYNxX_QN-o0WW1o752pQkBS_ohPEzObk/edit?slide=id.g3bd6f2d5062_0_178#slide=id.g3bd6f2d5062_0_178)

## Requirement
- In `initrd_list`, you should list all file's size and name.  
- In `initrd_cat`, detect whether the file exists.  
- Check `magic`.  

