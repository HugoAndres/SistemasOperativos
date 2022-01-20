#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define Nivel 2
#define Hijos 2

void infProcess(int lev){
	printf("Soy el proceso %d, mi padre es %d \n", getpid(), getppid(), lev);

}

void m_childs(int l){

	pid_t pidN;
	int status;

	for(int c = 1 ; c <= Nivel ; c++){

		pidN = fork();

		if (pidN > 0){

			continue;

		}else if(pidN == 0){

			l++;
			infProcess(l);

			if(l < Nivel)
				m_childs(l);

			exit(0);

		}else{

			printf("ERROR");

		}

	}

	for(int c = 0; c < Hijos  ; c++){

		pidN = wait(&status);
		//printf("Soy el proceso %d hijo terminado %d status %d \n\n", getpid(), pidN, WEXITSTATUS(status));

	}

	while(1)
		sleep(20);

}

void main(){

//	printf("Proceso PADRE %d\n\n", getpid());

	m_childs(0);
}
