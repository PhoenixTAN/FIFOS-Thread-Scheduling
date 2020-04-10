# FIFOS 2

Round Robin thread scheduler by Ziqi Tan & Jiaqian Sun

## How to run?
```
make clean
make
make install
qemu-system-i386 -kernel FIFOS-2-FINAL/fifos2
./root/vnc/opt/TigerVNC/bin/vncviewer :5900
```

## Project structure
- boot.s
  - Define GDT and IDT.
  - Initiate IDT table including the timer interrupt
- context_switch.s
  - protect and switch the context.
- fifos2.c
  - create_thread(): create one thread and initialize its fake context.
  - thread_finish(): used when a thread finish doing all jobs.
  - thread_run(): each thread has its own run method to finish their own tasks.
  - scheduler(): has a queue data structure to assign resource to specific thread.
  - switch_thread(): call the assembly code to switch the next thread and store the status of current thread.
- fifos2.ld
- interrupt.h
  - initialize PIT
  - initialize PIC
  - initialize the timer interrupt handler
- print.h
  - encapsulate the functions to print something on the terminal.
- multiboot.h
  - Define some multiboot data structure.
- thread.h
  - Define the TCB structure.
- Makefile

## Program flow
1. Program begins in **boot.s** and then **C** code is called;
2. **init()** in fifos2.c is being excuted;
3. Initialize the **ready queue**.
4. Create $N$ threads and their **TCBs**.
   1. Set up thread ID.
   2. Set up private stack space.
   3. Set up a task (the corresponding function).
   4. set up thread status as NEW.
   5. Set up **initial context in stack**.
5. Initialize PIC
6. Initialize PIT
7. Activate the interrupt

## Context protection and retrieve
In our implementation, we cannot directly modify the **IP** register, thus we recursively call the scheduler().

### Trick
Comparing with FIFOS-1, each thread cannot activally call yield function. Each thread will be interruptted by the system clock. In order to print out the information in sequence, when the thread tries to print out the remained jobs, we will block the interrupt, after the thread finishes writing the information on the screen, we will activate the interrupt.
