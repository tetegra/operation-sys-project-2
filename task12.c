#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int SharedVariable = 0;
pthread_barrier_t barrier;

void* SimpleThread(void* which) { 

	int num, val;
	int threadId = (int)which;
	printf("value of threadId is %d\n",threadId);
	
	for(num = 0; num < 20; num++) { 
       		if (random() > RAND_MAX / 2){ 
       		usleep(10);
 		}

		val = SharedVariable;

 		printf("*** thread %d sees value %d\n", threadId,val); 
		SharedVariable = val + 1;
	}
	pthread_barrier_wait(&barrier);
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", threadId, val);
	pthread_exit(NULL);
}

int main(int argc, char **argv){
	
	int count = atoi(argv[1]);
	pthread_barrier_init(&barrier,NULL,count);
	pthread_t thr[count];		//thread's ID
	int rc;		//return value
	int i;
	
	for (i = 0; i < count; i++){
		printf("value of i is %d\n",i);
		if ((rc = pthread_create(&thr[i], NULL, SimpleThread,(void*)i))){
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	for (i=0;i<count;i++){
		pthread_join(thr[i],NULL);
	}
		

	return EXIT_SUCCESS;
}	
