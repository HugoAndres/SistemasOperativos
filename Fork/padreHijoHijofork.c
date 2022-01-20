#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main ()
{
  pid_t idProceso;
  pid_t idProceso2;
  int esperaHijo = 0;

  idProceso = fork ();
  switch (idProceso)
    {
    case -1:
      printf ("Error\n");
      exit (1);
      break;
    case 0:
      printf ("Yo soy el hijo y mi ID es %d; mi padre es %d\n", getpid (),
	      getppid ());

      idProceso2 = fork ();
      switch (idProceso2)
	{
	case -1:
	  printf ("Error\n");
	  exit (1);
	  break;
	case 0:
	  printf ("Yo soy el hijo y mi ID es %d; de mi padre es %d\n",
		  getpid (), getppid ());
	  exit (1);
	  break;
	default:
//	  wait(&esperaHijo);
	  printf ("Yo soy el proceso padre y mi ID es: %d\n", getpid ());
	  exit (1);
	  break;
	}
    default:
//      wait(&esperaHijo);
      printf ("Yo soy el proceso padre y mi ID Es:%d\n", getpid ());
      break;
    }
}
