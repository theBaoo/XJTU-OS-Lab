# Makefile - 构建系统文件
RISCV_PREFIX = riscv64-unknown-elf-
CC = $(RISCV_PREFIX)gcc
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
CFLAGS = -Wall -O2 -fno-builtin -nostdlib -nostartfiles -mcmodel=medany -march=rv64imac_zicsr -mabi=lp64 -g
LDFLAGS = -T kernel.ld
QEMU := qemu-system-riscv64
QFLAGS := -machine virt -kernel kernel.bin -nographic -bios none

# There will be no output. I don't know why.
# SRCC = $(wildcard *.c)
# SRCS = $(wildcard *.S)
# OBJS = $(SRCC:.c=.o)
# OBJS += $(SRCS:.S=.o)
# SRCDIR := kernel
# SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.S)
# OBJS = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(filter %.c,$(SRCS))) \
#        $(patsubst $(SRCDIR)/%.S, $(SRCDIR)/%.o, $(filter %.S,$(SRCS)))
# It works.
OBJS = \
	entry.o \
	start.o \
	main.o \
	putc.o \
	printf.o \
	kinit.o \
	proc.o \
	swtch.o \
	trap.o \
	timer.o \
	strap.o \
	syscall.o \
	spinlock.o \
	PLIC.o \
	utrap.o \
	user.o \
	usys.o \
	sh.o \
	init.o \
	# swtchu.o

all: kernel.elf qemu

qemu: kernel.elf
	$(QEMU) $(QFLAGS)

qemu-gdb: kernel.elf
	$(QEMU) $(QFLAGS) -S -s

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ kernel.bin

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o kernel.elf kernel.bin