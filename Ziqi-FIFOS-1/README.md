# FIFOS 1

First come first serve thread scheduler.

## How to run?
```
make clean
make
make install
qemu-system-i386 disk.img
./root/vnc/opt/TigerVNC/bin/vncviewer :5900
```

## Project structure
- boot.s
  - Define GDT and IDT.
- context_switch.s
  - take care of the context protection and retrieve.
- print.h
  - encapsulate the functions to print something on the terminal.
- thread.h
  - Define the TCB structure.
- multiboot.h
  - Define some multiboot data structure.
- fifos1.ld
- Makefile

## Program flow
1. Program begins in **boot.s** and then **C** code is called;
2. **init()** in fifos1.c is being excuted;
3. Initialize the **ready queue**.
4. Create $N$ threads and their **TCBs**.
   1. Set up thread ID.
   2. Set up private stack space.
   3. Set up a task (the corresponding function).
   4. set up thread status as NEW.
   5. Set up **initial context in stack**.
5. call scheduler().
   1. Add all NEW theads into the ready queue.
   2. If the ready queue is empty, program ends.
   3. Get the first thread in the ready queue and execute.
   4. Context protection:
      1. Move the stack pointer of the **last thread** into **%ESI**.
      2. Move the stack pointer of the **next thread** into **%EDI**.
      3. push current registers into stack.
      4. pop registers for the next thread.
   5. Thread is running and then **yield()**.
      1. yield() adds the thread itself back to ready queue.
      2. Then, recursively call scheduler().
   6. 