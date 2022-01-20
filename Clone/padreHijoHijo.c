#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int
procesoHijoUno (void *arg)
{
  printf ("Soy el proceso nieto con ID:  %d,mi padre es: %d \n", getpid (),
	  getppid ());
  return 0;
}

int
procesoHijoDos (void *arg)
{
  char *stack;
  stack = (char *) malloc (1024 * 1024);
  char msg[100];
  pid_t idProcesoDos;
  int estado = 0;

  idProcesoDos = clone (procesoHijoUno, stack + (1024 * 1024), SIGCHLD, msg);
  if (idProcesoDos > 0)
    {
      wait (&estado);
      printf ("Soy el proceso hijo con ID: %d, mi padre es: %d \n", getpid (),
	      getppid ());
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  char *stack;
  stack = (char *) malloc (1024 * 1024);
  char msg[100];
  pid_t idProcesoUno;
  int estado = 0;
  idProcesoUno = clone (procesoHijoDos, stack + (1024 * 1024), SIGCHLD, msg);
  if (idProcesoUno > 0)
    {
      wait (&estado);
      printf ("Soy el proceso padre: %d \n", getpid ());
    }
  return 0;

}
