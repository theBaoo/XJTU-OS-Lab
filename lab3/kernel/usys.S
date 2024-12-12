.globl uwrite
.globl uread
.globl ufork
.globl uexec
.globl uexit
.globl uwait
.globl usleep
.globl ups
uwrite:
        # write system call
        # a0: file descriptor
        # a1: buffer
        # a2: size
        # return: size written
        li a7, 1
        ecall
        ret

uread:
        # read system call
        # a0: file descriptor
        # a1: buffer
        # a2: size
        # return: size read
        li a7, 2
        ecall
        ret
ufork:
        # fork system call
        # return: child pid
        li a7, 3
        ecall
        ret
uexec:
        # exec system call
        # a0: file name
        # a1: argv
        # return: -1 if failed
        li a7, 4
        ecall
        ret
uexit:
        # exit system call
        # a0: exit code
        li a7, 5
        ecall
        ret
uwait:
        # wait system call
        # a0: pid
        # a1: status
        # return: pid
        li a7, 6
        ecall
        ret
usleep:
        # sleep system call
        # a0: time in milliseconds
        li a7, 7
        ecall
        ret
ups:
        li a7, 8
        ecall
        ret