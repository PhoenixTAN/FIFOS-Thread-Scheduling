CC = gcc
CFLAGS = -m32 -fno-builtin -fno-stack-protector -fno-strict-aliasing -fno-delete-null-pointer-checks -nostdinc -I. -g -Wall -std=c99
CPPFLAGS = -Wa,--32 -MMD
OBJS = boot.o fifos1.o switch.o create.o temp.o
PROGS = fifos1
MNT_POINT=/root/fifos/c2disk

all: $(PROGS)

fifos1: $(OBJS)
	$(LD) -m elf_i386 -T fifos1.ld -o $@ $^

%: %.c

install: $(PROGS)
	cp $(PROGS) $(MNT_POINT)/boot
	sync

clean:
	-rm *.o *.d $(PROGS) *~

-include *.d
