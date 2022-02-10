#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef  int flag_t;

typedef enum {OK, FAIL} ret_t;

typedef struct {
	flag_t flag;
	ret_t  ret;
} arg_t; 

void* fun1(void* args) {
	arg_t* pargument = args;
	while(pargument->flag) {
		putchar('1');
		fflush(stdout);
		sleep(1);		
	}
	pargument->ret = OK;
	pthread_exit(&pargument->ret);
}

void* fun2(void* args) { 
	arg_t* pargument = args;
	while(pargument->flag) {
		putchar('2');
		fflush(stdout);
		sleep(1);
	}
	pargument->ret = OK;
	pthread_exit(&pargument->ret);
}

int main() {
	puts("Main thread started working.");
	arg_t arg1, arg2;
	arg1.flag = 1;
	arg2.flag = 1;
	pthread_t t1, t2;
	int retc1, retc2;
	retc1 = pthread_create(&t1, NULL, fun1, &arg1);
	if(retc1 == 0)
		puts("Routine fun1 started working");
	else {
		printf("Failed to start fun1 routine. %s\n", strerror(retc1));
		return 1;
	}
	
	retc2 = pthread_create(&t2, NULL, fun2, &arg2);
	if(retc2 == 0) 
		puts("Routine fun2 started working");
	else {
		printf("Failed to start fun2 routine.%s\n", strerror(retc1));
		arg1.flag = 0;
		int join_retc = pthread_join(t1 , NULL);
		if(join_retc == 0)
			puts("Routine fun1 stopped working");
		else
			printf("Failed to join routine fun1. %s", strerror(join_retc));
		puts("Main thread ending working");
		return 1;
	}
	puts("Press enter to stop two threads.");
	getchar();
	puts("Enter pressed.");
	arg1.flag = 0;
	arg2.flag = 0;
	ret_t* ret1;
       	retc1 = pthread_join(t1, (void**)&ret1);
	if(retc1 == 0) 
		puts("Routine fun1 ended working.");
       	else {
		printf("Failed to join fun1 routine. %s\n", strerror(retc1));	
		puts("Main thread ending working");
		return 1;
	}
	ret_t* ret2;
       	retc2 = pthread_join(t2, (void**)&ret2);
	if(retc2 == 0)
		puts("Routine fun2 ended working.");
	else {
		printf("Failed to join fun2 routine. %s\n", strerror(retc2));
		puts("Main thread ending working");
		return 1;
	}
	if(*ret1 == OK && *ret2 == OK)
		puts("Both threads returned OK code.");
	else {
		puts("Error occured! Not all threads returned OK code.\n");
		puts("Main thread ending working");
		return 1;
	}
	puts("Main thread ending working");
	return 0;

}
