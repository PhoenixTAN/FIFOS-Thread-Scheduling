# FIFOS
By Rich West, Boston University.

## Overview
In this assignment you are required to take the protected mode code you developed from MemOS (notably memos-2) and develop a new version of the system, which we will call **FIFOS (the First-In-First-Out System)**. 

FIFOS extends memos-2 with capabilities to **schedule threads in a FIFO (or FCFS) order**. That is, any newly created or awoken threads are added to the back of a ready queue and when the scheduler is invoked, it picks the thread at the front of the ready queue for dispatching. 

This assignment requires you to build **a standalone system rather than a solution on top of an existing system**, so you **cannot** use pre-existing thread libraries such as Pthreads or a runtime environment such as a Java Virtual Machine (although you can implement your own versions of Pthreads or a JVM as part of your system if you want to give yourself a challenge!).

## Details
**Using code from memos-2** that you developed for your primer assignment, you should start by adding functionality to support thread scheduling. For those who did not complete memos-2 we will provide template source code. You should extend the code to **allow threads to work within the default protection domain** that is created when GRUB passes control to your system. For simplicity, you can assume **a pool of N threads** is created statically **(where N is specified in a header or some boot parameter of your choosing).** You can assume the **thread pool exists at boot time, and no further threads need to be created dynamically**. 

For each thread, you should define a **thread control block (TCB)**, which maintains appropriate state information. Example state information includes copies of 
- **machine register** values such as the stack and instruction pointers (possibly others too), and 
- **a thread ID (TID)**. Each TID should be a unique integer value in the range [1,N] where N is the number of threads in your pool. 
- The pool itself might simply be an array of pointers to TCBs, with some flag identifying whether or not a thread in the pool has been assigned work.

Initially, threads in your thread pool are all idle and they all have the same priority. You can then assign threads specific work by calling a function thread_create(). The function should look something like:

int thread_create (void *stack, void *func);

In the above, the thread creation function binds a thread in the pool to specific stack and function addresses. It then returns the TID of the thread associated with this call. For simplicity you can assume threads are taken from the pool in order from lowest to highest TID until all threads have been assigned work. Each thread assigned work is then marked as busy using a flag in the corresponding TCB. If all threads are busy, you cannot create any more threads and should deal with this case accordingly. If a thread returns from the function it is associated with, you can place it back into an idle state, so that it can be assigned new functionality via a subsequent thread_create() call.

