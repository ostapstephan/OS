#ifndef CV_H
#define CV_H
#define CV_MAXPROC 64

struct cv{
    pid_t pArr[CV_MAXPROC];
    int filled;
    struct spinlock sp;
};

void cv_init(struct cv *cv);
void cv_wait(struct cv *cv,struct spinlock *mutex);
int cv_broadcast(struct cv *cv);
int cv_signal(struct cv *cv);

#endif //CV_H
