CC := gcc
FLAGS := -Wall -Werror

.PHONY: all clean process thread

all: process thread spinlock

process: system_call process_1 process_2 process_3 process_4 process_5

process_1: process_1.c
	@echo
	@echo "======Stage 1======"
	@$(CC) $(FLAGS) -o $@ $<
	@echo "Run 5 times:"
	@for i in {1..5}; do ./$@; echo; done
	@echo "Stage 1 finished."

process_2: process_1.c
	@echo "======Stage 2======"
	@$(CC) $(FLAGS) -o $@ $< -DSTAGE_2
	@echo "Run 5 times without wait(NULL):"
	@for i in {1..5}; do ./$@; echo; done
	@echo "Stage 2 finished."

process_3: process_2.c
	@echo "======Stage 3======"
	@$(CC) $(FLAGS) -o $@ $<
	@echo "Run 5 times with a shared value:"
	@for i in {1..5}; do ./$@; echo; done
	@echo "Stage 3 finished."

process_4: process_2.c
	@echo "======Stage 4======"
	@$(CC) $(FLAGS) -o $@ $< -DSTAGE_4
	@echo "Run 5 times with operaion before return:"
	@for i in {1..5}; do ./$@; echo; done
	@echo "Stage 4 finished."

process_5: process_3.c
	@echo "======Stage 5======"
	@$(CC) $(FLAGS) -o $@ $< -DSYS
	@echo "system"
	@./$@

	@echo

	@$(CC) $(FLAGS) -o $@ $< -DEXEC
	@echo "exec"
	@./$@
	@echo "Stage 5 finished."

system_call: system_call.c
	$(CC) $(FLAGS) -o system_call system_call.c


thread: system_call thread_1 thread_2 thread_3 thread_4

thread_%: thread.c
	@echo 
	@echo "======STAGE $*======"
	@$(CC) $(FLAGS) -o $@ $< -lpthread -DSTAGE_$*
	@echo "Run 5 times:"
	@for i in {1..5}; do ./$@; echo; done
	@echo "Stage $* finished."


spinlock: spinlock.c
	@echo 

	@echo "======Unlock======"
	@$(CC) $(FLAGS) -o $@ $< -lpthread
	@for i in {1..5}; do ./$@; echo; done

	@echo "======Locked======"
	@$(CC) $(FLAGS) -o $@ $< -DSPIN_LOCK -lpthread
	@for i in {1..5}; do ./$@; echo; done

clean:
	find . -type f -executable -delete


# thread_1: thread.c
# 	@echo "======STAGE 1======"
# 	@$(CC) $(FLAGS) -o $@ $< -lpthread -DSTAGE_1
# 	@echo "Run 5 times:"
# 	@for i in {1..5}; do ./$@; done
# 	@echo "Stage 1 finished."

# thread_2: thread.c
# 	@echo "======STAGE 2======"
# 	@$(CC) $(FLAGS) -o $@ $< -lpthread -DSTAGE_2
# 	@./$@
# 	@echo "Stage 2 finished."

# thread_3: thread.c
# 	@echo "======STAGE 3======"
# 	@$(CC) $(FLAGS) -o $@ $< -lpthread -DSTAGE_3
# 	@./$@
# 	@echo "Stage 3 finished."

# thread_4: thread.c
# 	@echo "======STAGE 4======"
# 	@$(CC) $(FLAGS) -o $@ $< -lpthread -DSTAGE_4
# 	@./$@
# 	@echo "Stage 4 finished."