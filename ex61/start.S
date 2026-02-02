.section ".text.boot"
.global _start
_start:
    la a2, 1f
    li t0, 0xffffffc000000000
    add a2, a2, t0
    csrw stvec, a2
    la sp, _end
    call setup_vm
.align 2
1:
    la a3, __bss_start
    la a4, __bss_stop
    ble a4, a3, clear_bss_done
clear_bss:
    sd zero, (a3)
    add a3, a3, 8
    blt a3, a4, clear_bss
clear_bss_done:
    la sp, _end
    tail start_kernel
