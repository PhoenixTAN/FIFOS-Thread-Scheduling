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

## Context protection and retrieve
In our implementation, we cannot directly modify the **IP** register, thus we recursively call the scheduler().

### Trick
When a thread is called for the first time, when a **ret** happens: **IP=SS:SP**. What we only need to do is make SP (stack pointer) to the **thread_run()** function.

And then every threads will be scheduled in a certain flow. We don't need to manually protect the **IP** register. We still need to take care of general registers and the flag register.
