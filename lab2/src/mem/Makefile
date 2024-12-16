include ../../../common.mk

.PHONY: test clean

# TEST := $(wildcard testbench*)
TEST := testbenchBF testbenchFF testbenchWF

mem: mem.c
	$(CC) $(CFLAGS) -o $@ $^
# auto test
mem_test: mem.c
	$(CC) $(CFLAGS) -o $@ $^ -DTEST
test: mem_test generate
	@for file in $(TEST); do echo "test $$file"; ./mem_test < $$file > $$file.result; done
generate: generate.c
	$(CC) $(CFLAGS) -o $@ $^
	./$@
debug: mem
	gdb $< -x init.py

clean:
	find . -type f -executable -delete
	rm -f *.result