CC = gcc
CFLAGS = -m32 -fno-builtin -fno-stack-protector -fno-strict-aliasing -fno-delete-null-pointer-checks -nostdinc -I. -g -Wall -std=c99
CPPFLAGS = -Wa,--32 -MMD
OBJS = boot.o fifos2.o context.o
PROGS = fifos2
MNT_POINT=/root/fifos/mydisk

all: $(PROGS)

fifos2: $(OBJS)
	$(LD) -m elf_i386 -T fifos2.ld -o $@ $^

%: %.c

install: $(PROGS)
	cp $(PROGS) $(MNT_POINT)/boot
	sync

clean:
	-rm *.o *.d $(PROGS)

-include *.d
