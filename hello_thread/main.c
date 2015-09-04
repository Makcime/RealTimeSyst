#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

static void * hello(){
	printf("hello thread !\n");
	return;
}

int main(int argc, char const *argv[])
{
	// int pthread_create (pthread_t * thread, pthread_attr_t * attr, void * (* start_routine) (void *), void * arg); 
	int ret;
	pthread_t mon_thread;

	ret = pthread_create(& mon_thread, NULL, hello, NULL);

	/* Attente de la fin des threads. */

	pthread_join (mon_thread, NULL);
 
   return EXIT_SUCCESS;
}