CC=gcc
ARCH=riscv32-unknown-linux-gnu-

#CFLAGS=-Wall -Werror -pedantic -std=c11 -O0 -g
CFLAGS=-Wall -Werror -pedantic -std=c11 -O0
FILES=box.c util.c locks/lock1.c locks/lock2.c locks/lock3.c locks/lock4.c locks/lock5.c locks/lock6.c locks/lock7.c

.PHONY: init r2 gdb-lock1 gdb-lock2 gdb-lock3 gdb-lock4 gdb-lock5 gdb-lock6
build:
	$(ARCH)$(CC) $(FILES) $(CFLAGS) $(LGEM) $(L1) $(L2) $(L3) $(L4) $(L5) $(L6) $(L7) -o puzzlebox
	$(CC) $(FILES) $(CFLAGS) $(LGEM) $(L1) $(L2) $(L3) $(L4) $(L5) $(L6) $(L7) -o puzzlebox_tester

init:
	chmod +x puzzlebox*

r2:
	r2 -A puzzlebox
