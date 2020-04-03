/* Ready queue */

#define ready_queue_size    1024

typedef struct {
    int tid[ready_queue_size];
    int head;
    int tail;
} ready_queue;



