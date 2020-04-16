#include "io.h"
#include "thread.h"
#include "interrupt.h"

#define N 6
#define FIB_MAX 10

void fib()
{
    unsigned tid = get_tid();
    int fib[FIB_MAX] = {0, 1};
    int i = 2;
    for (; i < FIB_MAX; ++i)
    {
        fib[i] = fib[i - 2] + fib[i - 1];
        yield();
    }
    for (i = 0; i < FIB_MAX; ++i)
    {
        unsigned j = 0;
        for (; j < 0xFFFFFu; ++j)
            ;
        printd(tid);
        prints(": Fib[");
        printd(i);
        prints("]=");
        printd(fib[i]);
        prints("\t");
        // yield();
    }
    prints("Done <");
    printd(tid);
    prints(">\t");
}

void init(unsigned *binfo)
{
    init_heap();
    init_pic();
    init_pit(11932); // 10ms interval
    init_thread(N);
    unsigned mem_upper = binfo[2];
    clear();
    prints("FIFOS: Welcome *** System Memory is: ");
    unsigned fraction = mem_upper & ~(~0 << 10);
    printd((mem_upper >> 10) + (fraction >= (1 << 9) ? 2 : 1));
    prints("MB\n");
    int i = 1;
    for (; i <= N; ++i)
    {
        int tid = thread_create(fib);
        if (tid)
        {
            prints("Thread ");
            printd(tid);
            prints(" created.\n");
        }
        else
        {
            prints("Failed to create new thread.\n");
            break;
        }
    }
    enable_interrupt();
    yield();
    disable_interrupt();
    prints("\nAll threads finished.");
}
