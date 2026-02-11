# Devicetree Parsing

## Exercise2 Slide
[OSC2026_lab2_exercise](https://docs.google.com/presentation/d/1_WRWjumH9A3CYNxX_QN-o0WW1o752pQkBS_ohPEzObk/edit?slide=id.g3bd6f2d5062_0_56#slide=id.g3bd6f2d5062_0_56)

## Intro.
### Devicetree
- A data structure describing hardware, allowing the kernel to find devices without hardcoded addresses.
- You can get the specification [here](https://www.devicetree.org/specifications).

### Devicetree Format
1. Devicetree source (.dts): human-readable form
1. Flattened devicetree (.dtb): compiled format for parsing

## Flattened devicetree (.dtb)
| Structure | 
| ------ | 
| FDT Header | 
| Memory Reservation Block | 
| Structure Block | 
| Strings Block |

### FDT Header
```cpp
struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};
```
>[!Tip] You may use these informations:
    - `magic`: 0xd00dfeed
    - `off_dt_struct`
    - `off_dt_strings`

### Structure Block

| Token          | ID  | Description                   |
| -------------- | --- | ----------------------------- |
| FDT_BEGIN_NODE | 0x1 | Node_name with '\0' + Padding |
| FDT_END_NODE   | 0x2 | End of the node               |
| FDT_PROP       | 0x3 | Prop_len + Prop_name offset in Strings Block + Padding |
| FDT_NOP        | 0x4 | Ignored                       | 
| FDT_END        | 0x9 | End of the devicetree         | 
>[!Note] Padding: align up to 4 bytes

## TODO
- `fdt_path_offset`: Return the offset of a node by path.
- `fdt_getprop`: Return the pointer to property data.
- [Expected result](https://docs.google.com/presentation/d/1_WRWjumH9A3CYNxX_QN-o0WW1o752pQkBS_ohPEzObk/edit?slide=id.g3bd6f2d5062_0_92#slide=id.g3bd6f2d5062_0_92)

## Requirement
- In `fdt_path_offset`, you should return the first match node.
- Check `magic`.  

