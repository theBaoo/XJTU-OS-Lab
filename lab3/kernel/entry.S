# entry.S - 内核入口汇编代码
.section .text
.global _entry
_entry:
    # 设置栈指针
    la sp, stack0
    li a0, 4096
    csrr a1, mhartid
    mul a0, a0, a1
    add sp, sp, a0

    # 跳转到 start 函数
    call start

spin:
    j spin