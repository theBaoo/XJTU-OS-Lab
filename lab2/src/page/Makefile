include ../../../common.mk

SRC := $(wildcard *.c)

page: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

debug: page
	gdb ./$<

*.o: *.c
	$(CC) $(CFLAGS) -c $<

clean:
	find . -type f -executable -delete