include ../../../common.mk

interrupt: interrupt.c
	$(CC) $(CFLAGS) -o $@ $^

debug: interrupt
	gdb ./interrupt

clean:
	find . -type f -executable -delete