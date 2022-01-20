/*Un proceso creara un nuevo proceso*/
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int
main (void)
{
  int espera = 0;
  int i = 0;
  pid_t idProceso;		/*identificador de proceso linux. */
  idProceso = fork ();		/*crea un nuevo proceso. hijo -> padre. */
  if (idProceso > 0)		/*se esta ejecutando el proceso padre. */
    {
      printf ("Yo soy el proceso padre, y mi ID es:%d.\n", getpid ());
      wait (&espera);
      /* --> waitipd(), tenemos que decir exactamente el id del hijo que queremos esperar.
         --> wait(), no tiene que especificar el id del proceso hijo, el padre solo espera la finalizacion del proceso hijo. */
    }
  else if (idProceso == 0)	/*se estara ejecutando el hijo. */
    {
      sleep (3);		/*El proceso padre debe esperar que el proceso hijo termine.(con sleep muere primero el proceso padre) */
      /*cuando el proceso padre muere, el hijo adopta otro proceso padre. */
      printf ("Yo soy el proceso hijo (1) , y mi ID es:%d , el ID de mi padre es: %d \n", getpid (), getppid ());
    }
  else
    {
      printf ("Error en el proceso.");	//error(-)
    }

  return 0;
}
