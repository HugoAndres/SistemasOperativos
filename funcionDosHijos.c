#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void main(void){
        int  i = 0;
	int esperaHijo = 0;
        pid_t idProceso;
	for(i=0;i<=1;i++){
                idProceso = fork();
		wait(&esperaHijo);
//		printf("Valor de fork(%d) = %d \n",i ,idProceso);
                switch(idProceso)
                {
                        case -1:

                                printf("Error\n");
                                exit(1);
                        break;

                        case 0:
                                if(i == 0){
//					sleep(3);
                                        printf("Soy el hijo (1) y mi ID es: %d y mi padre es %d\n", getpid(), getppid() );
                                }
                                else{
//					sleep(3);
                                        printf("Soy el hijo (2) y mi ID es: %d y mi padre es %d\n", getpid(), getppid() );
                                }
                        break;

                        default:
                                printf("Yo soy el padre y mi PID es: %d\n", getpid());
				exit(0);
                        break;
		}
	}
}

