/**
 * Author: Ziqi Tan, Jiaqian Sun
 * 
 * Reference: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
*/

#include "multiboot.h"
#include "thread.h"
#include "print.h"
// #include "schedule.h"

/* Macros. */
/* The number of threads */ 
#define N                       3
#define stack_size              1024
#define ready_queue_size        256

/* Variables */
static TCB tcb[N];
static int uid = 0;     // counter
static int curUid = -1;
static int callMainThreadTime = 0;

/* data structure and operationg for ready queue */
static TCB* ready_queue[ready_queue_size];
static int head;
static int tail;
void init_ready_queue();
int en_queue(TCB* tcb);
TCB* de_queue();
int isEmpty();
int isFull();

/* Private stack for threads */
multiboot_uint32_t stack1[stack_size];
multiboot_uint32_t stack2[stack_size];
multiboot_uint32_t stack3[stack_size];

multiboot_uint32_t main_thread_stack[stack_size];

/* Forward declarations. */
void init(multiboot_info_t* pmb);       
int thread_create(void* stack, void* run);
void thread1_run();
void thread2_run();
void thread3_run();
void fifo_scheduler();
void thread_exit();
void thread_yeild();
void create_main_thread(void* stack, void* run);

/* entrance of the C code */
void init(multiboot_info_t* pmb) {
    cls();
    println("FIFOS-1: Ziqi Tan, Jiaqian Sun");
    println("First come first serve thread scheduler:");
    
    /* create main thread */
    create_main_thread(&main_thread_stack[stack_size-1], fifo_scheduler);

    init_ready_queue();

    /* create threads */
    create_therad(&stack1[stack_size-1], thread1_run);
    create_therad(&stack2[stack_size-1], thread2_run);
    create_therad(&stack3[stack_size-1], thread3_run);

    fifo_scheduler();
}

/* Create thread */
int create_therad(void* stack, void* run) {
    // index out of bound exception
    if( uid > N ) {
        return -1;
    }
    tcb[uid].sp = (multiboot_uint32_t)stack;
    tcb[uid].tid = uid + 1;
    tcb[uid].run = run;
    tcb[uid].status = NEW;
    tcb[uid].priority = 0;
    *((multiboot_uint32_t *)stack) = (multiboot_uint32_t)run;
    // *((multiboot_uint32_t *)stack -  1) = 2;
    en_queue(&tcb[uid]);
    uid = uid + 1;
    return tcb[uid].tid;
}

//Use to create the main thread, the pointer of fifo_scheduler will be push into its stack, during the switch context, we call ret in the 
//assembly to call the fifo_scheduler function.
void create_main_thread(void* stack, void* run) {
    *((multiboot_uint32_t *)stack) = (multiboot_uint32_t)run;
    // *((multiboot_uint32_t *)stack -  1) = 2;
}

/* */
void thread1_run() {
    int i = 6;
    while(i >= 0) {
        println("Thread<0001> is running...");
        i--;
        thread_yeild();
    }
    thread_exit();
    // print from 100 to 200
}

/* */
void thread2_run() {
    int i = 6;
    while(i >= 0) {
        println("Thread<0002> is running...");
        i--;
        thread_yeild();
    }
    thread_exit();
    // print from 200 to 300
}

/* */
void thread3_run() {
    int i = 6;
    while(i >= 0) {
        println("Thread<0003> is running...");
        i--;
        thread_yeild();
    }
    thread_exit();
    // print from 300 to 400
}

/* First come first serve scheduler */
void fifo_scheduler() {
    // (1) context protection

    // (2) tcb.status = READY

    // (3) add to ready queue or this thead has been terminated

    // (4) poll the first in the ready queue

    // (5) context restore
    println("scheduler start");
    if(!isEmpty()) {
        TCB *ptr = de_queue();
        if(curUid == -1 && (*ptr).status == TERMINATED) {
            __asm__ volatile("call switch_to"::"S"(0), "D"(ptr));
        }
        else if(curUid != -1 && (*ptr).status == TERMINATED) {
            __asm__ volatile("call switch_to"::"S"(&tcb[curUid]), "D"(ptr));
        }
        if(curUid == -1 && (*ptr).status == NEW) {
            __asm__ volatile("call create"::"S"(0), "D"(ptr));
        }
        else if(curUid != -1 && (*ptr).status == NEW) {
            __asm__ volatile("call create"::"S"(&tcb[curUid]), "D"(ptr));
        }
        curUid = (*ptr).tid;
        (*ptr).status = RUNNING;
    }
    else {
        println("In the end");
        __asm__ volatile("jmp finish_loop");
    }
}
// 当一个线程结束时调用的方法
void thread_exit() {
    TCB *ptr = &tcb[curUid];
    (*ptr).status = TERMINATED;
    if(callMainThreadTime == 0) {
        callMainThreadTime++;
        // switch the context between current thread and main thread
        __asm__ volatile("call create"::"S"(ptr), "D"(main_thread_stack[stack_size - 1]));
    }
    else {
        __asm__ volatile("call switch_to"::"S"(ptr), "D"(main_thread_stack[stack_size - 1]));
    }
}

void thread_yeild() {
    TCB *ptr = &tcb[curUid];
    (*ptr).status = BLOCK;
    println("after block");
    en_queue(ptr);
    if(callMainThreadTime == 0) {
        callMainThreadTime++;
        println("before switch");
        __asm__ volatile("call create"::"S"(0), "D"(main_thread_stack[stack_size - 1]));
        println("after switch");
    }
    else {
        __asm__ volatile("call switch_to"::"S"(ptr), "D"(main_thread_stack[stack_size - 1]));
    }
}
/* Opeartions for ready queue */
void init_ready_queue() {
    head = 0;
    tail = 0;
}

/* add */
int en_queue(TCB* tcb) {
    if( isFull() == 1 ) {
        return -1;
    }
    ready_queue[tail] = tcb;
    tail = (tail + 1) % ready_queue_size;
    println("en_queue");
    return 0;
}

/* poll */
TCB* de_queue() {
    if( isEmpty() == 1 ) {
        return -1;
    }
    TCB* res = ready_queue[head];
    head = (head + 1) % ready_queue_size;
    println("de_queue");
    return res;
}

/* isEmpty */
int isEmpty() {
    if(head == tail ) {
        return 1;
    }
    return 0;
}

/* isFull */
int isFull() {
     if ((tail + 1) % ready_queue_size == head ) {
         return 1;
     }
     return 0;
}
