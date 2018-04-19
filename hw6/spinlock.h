#ifndef SPIN_H
#define SPIN_H
#include <unistd.h>
#include <sys/types.h>

struct spinlock{
    char lock;
    pid_t pidCurr;
};

void spin_lock(struct spinlock *l);
void spin_unlock(struct spinlock *l);
int tas(volatile char * lock);

#endif //_SPIN_H
