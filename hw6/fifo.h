#ifndef FIFO_H
#define FIFO_H

#define MYFIFO_BUFSIZE 1024

#include "spinlock.h"
#include "cv.h"

struct fifo{
    unsigned long buf[MYFIFO_BUFSIZE];
    int next_write,next_read;
    int itemCount;
    struct cv full, empty; 
    struct spinlock mutex;
};

void fifo_init(struct fifo *f);
void fifo_wr(struct fifo *f, unsigned long d);
unsigned long fifo_rd(struct fifo *f);

#endif //FIFO_H
