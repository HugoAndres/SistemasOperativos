#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int
procesoHijoUno (void *arg)
{
  printf ("Soy el hijo (1) con ID:  %d,mi padre es: %d \n", getpid (),
	  getppid ());
  return 0;
}

int
procesoHijoDos (void *arg)
{
  printf ("Soy el hijo (2) con ID:  %d,mi padre es: %d \n", getpid (),
	  getppid ());
  return 0;
}

int
main (int argc, char *argv[])
{
  char *stack;
  char msg[100];
  pid_t idProcesoUno;
  pid_t idProcesoDos;
  int estado = 0;
  stack = (char *) malloc (1024 * 1024);

  if (!stack)
    {
      fprintf (stderr, "No se asigno a pila");
      exit (1);
    }

  idProcesoUno = clone (procesoHijoUno, stack + (1024 * 1024), SIGCHLD, msg);
  if (idProcesoUno > 0)
    {
      idProcesoDos =
	clone (procesoHijoDos, stack + (1024 * 1024), SIGCHLD, msg);
      if (idProcesoDos > 0)
	{
	  wait (&estado);
	  wait (&estado);
	  printf ("Soy el proceso padre mi ID es: %d \n", getpid ());
	}
    }
  return 0;
}
