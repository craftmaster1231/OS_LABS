#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

typedef int flag_t;

flag_t flag1 = 1;
flag_t flag2 = 1;

sem_t screen_sem;

void* routine1(void* args) {
	while(flag1) {
		sem_wait(&screen_sem);
		for(int i = 0; i < 4; ++i) {
			if(flag1) {
				putchar('1');
				fflush(stdout);
			}
			else {
				sem_post(&screen_sem);
				return NULL;
			}
			struct timespec spec;
			spec.tv_sec = 0;
			spec.tv_nsec = 500000000;
			nanosleep(&spec, NULL);
		}
		sem_post(&screen_sem);
		struct timespec spec;
                spec.tv_sec = 0;
             	spec.tv_nsec = 500000000;
                nanosleep(&spec, NULL);

	}
	return NULL;
}

void* routine2(void* args) {
	while(flag2) {
		sem_wait(&screen_sem);
	        for(int i = 0; i < 4; ++i) {
			if(flag2) {
	                	putchar('2');
				fflush(stdout);
			}
			else {
				sem_post(&screen_sem);
				return NULL;
			}
			struct timespec spec;
        	        spec.tv_sec = 0;
                	spec.tv_nsec = 500000000;
	                nanosleep(&spec, NULL);

		}
        	sem_post(&screen_sem);
		struct timespec spec;
                spec.tv_sec = 0;
                spec.tv_nsec = 500000000;
                nanosleep(&spec, NULL);

	}
        return NULL;

}

int main() {
	sem_init(&screen_sem, 0, 1);
	puts("Main thread started.");
	pthread_t t1, t2;
	int status = pthread_create(&t1, NULL, routine1, NULL);
	assert(status == 0);
	puts("Routine 1 started.");
	status = pthread_create(&t2, NULL, routine2, NULL);
	assert(status == 0);
	puts("Routine 2 started.");
	puts("Press Enter to stop both threads.");
	getchar();
	puts("Enter pressed.");
	flag1 = 0;
	flag2 = 0;
	status = pthread_join(t1, NULL);
	assert(status == 0);
	puts("Routine 1 stopped.");
	status = pthread_join(t2, NULL);
	assert(status == 0);
	puts("Routine 2 stopped.");
	puts("Main thread ending to work.");
	sem_destroy(&screen_sem);
	return 0;
}
