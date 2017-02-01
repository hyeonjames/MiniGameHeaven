#include "timer.h"



pthread_t Thread(void (*func)(void*),void* args)
{
	int rc,status;
	pthread_t thread;
	pthread_create(&thread,NULL,func,args);
	rc = pthread_join(thread,(void**)&status);
	if(rc!=0) return NULL; // Fail
	else return thread;
}


int KillThread(pthread_t thread)
{
	return pthread_cancel(thread);
}

int TimerRestart(timer* t)
{
	struct timeval timenow;
	gettimeofday(&timenow, NULL);
	if(t==NULL) return 1;
	t->before = timenow;
	return 0;
}


void CreateTimer(long interval,timer* t)
{
    	gettimeofday(&t->start, NULL);
	gettimeofday(&t->before, NULL);
	t->interval = interval;
}

long Check(timer* t)
{
	long mtime,seconds,useconds;
	struct timeval timenow;
	if(t==NULL || t->interval < 0) return 0;
	gettimeofday(&timenow, NULL);
	seconds  = timenow.tv_sec  - t->before.tv_sec;
    	useconds = timenow.tv_usec - t->before.tv_usec;
    	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	if(mtime >= t->interval)
	{
		t->before = timenow;
		return 1;
	}
	else return 0;
}

long GetTime(timer* t)
{
	long mtime,seconds,useconds;
	struct timeval timenow;
	gettimeofday(&timenow, NULL);
	seconds  = timenow.tv_sec  - t->start.tv_sec;
    	useconds = timenow.tv_usec - t->start.tv_usec;
    	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	return mtime;
}
