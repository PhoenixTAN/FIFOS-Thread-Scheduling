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

/* data structure and operationg for ready queue */
static int ready_queue[ready_queue_size];
static int head;
static int tail;
void init_ready_queue();
int en_queue(int tid);
int de_queue();
int isEmpty();
int isFull();

/* Private stack for threads */
multiboot_uint32_t stack1[stack_size];
multiboot_uint32_t stack2[stack_size];
multiboot_uint32_t stack3[stack_size];

/* Forward declarations. */
void init(multiboot_info_t* pmb);       
int thread_create(void* stack, void* run);
void thread1_run();
void thread2_run();
void thread3_run();
void fifo_scheduler();

/* entrance of the C code */
void init(multiboot_info_t* pmb) {
    cls();
    println("FIFOS-1: Ziqi Tan, Jiaqian Sun");
    println("First come first serve thread scheduler:");
    
    /* create threads */
    create_therad(&stack1[stack_size-1], thread1_run);
    create_therad(&stack2[stack_size-1], thread2_run);
    create_therad(&stack3[stack_size-1], thread3_run);

    init_ready_queue();
    en_queue(100);
    en_queue(255);
    int res = de_queue();
    if( res == 100 ) {
        println("100");
    }
    if( de_queue() == 255 ) {
        println("get 255");
    }
    if( isEmpty() == 1 ) {
        println("Empty");
    }

}

/* Create thread */
int create_therad(void* stack, void* run) {
    // index out of bound exception
    if( uid > N ) {
        return -1;
    }
    tcb[uid].sp = stack;
    tcb[uid].tid = uid + 1;
    tcb[uid].run = run;
    tcb[uid].status = NEW;
    tcb[uid].priority = 0;
    uid = uid + 1;
    return tcb[uid].tid;
}

/* */
void thread1_run() {
    println("Thread<0001> is running...");
    // print from 100 to 200
}

/* */
void thread2_run() {
    println("Thread<0002> is running...");
    // print from 200 to 300
}

/* */
void thread3_run() {
    println("Thread<0003> is running...");
    // print from 300 to 400
}

/* First come first serve scheduler */
void fifo_scheduler() {
    // (1) context protection

    // (2) tcb.status = READY

    // (3) add to ready queue or this thead has been terminated

    // (4) poll the first in the ready queue

    // (5) context restore

}

/* Opeartions for ready queue */
void init_ready_queue() {
    head = 0;
    tail = 0;
}

/* add */
int en_queue(int tid) {
    if( isFull() == 1 ) {
        return -1;
    }
    ready_queue[tail] = tid;
    tail = (tail + 1) % ready_queue_size;
    println("en_queue");
    return 0;
}

/* poll */
int de_queue() {
    if( isEmpty() == 1 ) {
        return -1;
    }
    int res = ready_queue[head];
    head = (head + 1) % ready_queue_size;
    println("de_queue");
    return res;
}

/* isEmpty */
int isEmpty() {
    if(head == tail ) {
        return 1;
    }
    return -1;
}

/* isFull */
int isFull() {
     if ((tail + 1) % ready_queue_size == head ) {
         return 1;
     }
     return -1;
}