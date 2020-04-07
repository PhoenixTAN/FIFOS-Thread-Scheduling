#include "multiboot.h"

/* Thead status */
enum THREAD_STATUS {
    NEW, READY, RUNNING, BLOCK, TERMINATED
};

/* TCB(Thread Control Block) */
typedef struct {
    multiboot_uint32_t *sp;           // stack pointer
    int tid;                    // a thread uid
    multiboot_uint32_t (*run)();
    enum THREAD_STATUS status;
    int priority;

} TCB;


