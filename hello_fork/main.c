#include <stdio.h>

int main (void)
{

  /* afficher le mot bonjour et sauter une ligne */

  int pid;
  pid = fork();

  if(pid == 0){

  	printf("Je suis le fils  avec le pid: %d \n", pid);
  }
  else{
  	wait(NULL);
  	printf("Je suis le pere  avec le pid: %d \n", pid);
  }

  return 0;
}
