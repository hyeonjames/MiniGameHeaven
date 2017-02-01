#ifndef _TIMER_H
#define _TIMER_H
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#define FTIMER_ON 1
#define FTIMER_READY_EXIT 2
#define FTIMER_EXITED 4
#define FTIMER_DEFAULT 0
#define isTimerOn(f) f->status & FTIMER_ON
typedef struct
{
	struct timeval start, before;
	long interval;
}timer;

int TimerRestart(timer* t);

pthread_t Thread(void(*func)(void*),void*);
int KillThread(pthread_t);
void CreateTimer(long,timer*);
long Check(timer* t);
long GetTime(timer* t);
#endif

