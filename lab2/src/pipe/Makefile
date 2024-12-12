include ../../../common.mk

all:pipe_lock pipe_unlock

pipe_unlock: pipe.c
	$(CC) $(CFLAGS) -o $@ $^

pipe_lock: pipe.c
	$(CC) $(CFLAGS) -D__LOCK -o $@ $^

debug: pipe
	gdb ./$<

clean:
	find . -type f -executable -delete