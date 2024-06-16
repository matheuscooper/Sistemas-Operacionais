#include <stdio.h>
#include <unistd.h>
 
int main()
{  int r, pid;
   printf("antes da criacao\n");
   pid = fork();
   if (pid == 0)
      printf("eu sou o filho\n");
   else
      printf("eu sou o pai com filho pid = %d\n", pid);
   printf("dormindo...\n");
   sleep(5);   
   return 0;
}