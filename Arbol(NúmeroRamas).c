#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void dosHijos( int niveles, int guiones){
	int *espera, i;
	pid_t child;
	for(i = 0; i <=1 ; i++)
	{
		child = fork();
		switch(child)
		{

			case -1:

				printf("No se pudo crear el hijo\n");
				exit(1);

			break;

			case 0:
				for (int i = 0; i < guiones; ++i)
					{
						printf(">");
					}
				if(i == 0){
					printf("Soy el hijo izquierdo y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						guiones++;
						niveles--;
						dosHijos(niveles, guiones);
					}
				} 
				else{
					printf("Soy el hijo derecho y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						guiones++;
						niveles--;
						dosHijos(niveles, guiones);
					}
				}
				exit(0);
			break;

			default:
				printf("Yo soy el padre y mi PID es: %d\n", getpid());
			break;

		}
	}
	//for(i = 0; i <=1 ; i++) wait(espera);
	while(1);
}

void masHijos(int niveles, int guiones){
	int *espera, i;
	pid_t child;
	for(i = 0; i <=guiones ; i++)
	{
		child = fork();
		switch(child)
		{

			case -1:

				printf("No se pudo crear el hijo\n");
				exit(1);

			break;

			case 0:
				for (int i = 0; i < guiones; ++i)
					{
						printf("<");
					}
				if(i == 0){
					printf("Soy el hijo izquierdo y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						guiones++;
						niveles--;
						masHijos(niveles, guiones);
					}
				} 
				else{
					printf("Soy el hijo derecho y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						guiones++;
						niveles--;
						masHijos(niveles, guiones);
					}
				}
				exit(0);
			break;

			default:
				printf("Yo soy el padre y mi PID es: %d\n", getpid());
			break;

		}
	}
	//for(i = 0; i <=guiones ; i++) wait(espera);
	while(1);
}

int main()
{
	int niveles, guiones=1,*espera, i;
	pid_t child;
	printf("Ingrese el número de niveles del árbol\n");
	scanf("%d", &niveles);

	for(i = 0; i <=1 ; i++)
	{
		child = fork();
		switch(child)
		{

			case -1:

				printf("No se pudo crear el hijo\n");
				exit(1);

			break;

			case 0:
				if(i == 0){
					printf("1<");
					printf("Soy el hijo izquierdo y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						masHijos(niveles-2, guiones);
					} 
				} 
				else{
					printf("1>");
					printf("Soy el hijo derecho y mi ID es: %d Mi padre es %d\n", getpid(), getppid() );
					if (niveles>0)
					{
						dosHijos(niveles-2, guiones);
					} 
				}
				exit(0);
			break;

			default:
				printf("Yo soy el padre y mi PID es: %d\n", getpid());
			break;

		}
	}
	//for(i = 0; i <=1 ; i++) wait(espera);
	while(1);
}
