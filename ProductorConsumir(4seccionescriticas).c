#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define IPC_RESULT_ERROR (-1)
#define TAMANO_MEMORIA 5
#define RANGO 10000
#define PRODUCTORES 5
#define CONSUMIDORES 3

int *Memoria;

int BloqueCompartido();
int * AdjuntarMemoria();
int Des_Memo(int *);
int DestruirMemoria();

sem_t * CrearSemaforos (char *, int); 
void LimpiarSemaforos(char *, sem_t *); 

void GuardarProd(sem_t *, int, int, int, FILE *); 

int main()
{
    sem_t *ProdZona1, *ConsZona1, *ProdZona2, *ConsZona2; 
    sem_t *ProdZona3, *ConsZona3, *ProdZona4, *ConsZona4; 
    sem_t *Cont; 
    sem_t *Archivo; 
    pid_t pid; 
    char *NomProd1, *NomCons1, *NomProd2, *NomCons2;  
    char *NomProd3, *NomCons3, *NomProd4, *NomCons4;  
    char *NomCont; 
    char *NombreArch;  
    int flagZona1, flagZona2, flagZona3, flagZona4; 
    int a; 
    int x, y; 
    int last; 
    int p; 
    FILE *doc; 
    
    DestruirMemoria(Memoria);
    
    //Inicialización de Variables
    NomCons1 = "ConstructorZona1";
    NomProd1 = "ProductorZona1";
    NomCons2 = "ConstructorZona2";
    NomProd2 = "ProductorZona2";
    NomCons3 = "ConstructorZona3";
    NomProd3 = "ProductorZona3";
    NomCons4 = "ConstructorZona4";
    NomProd4 = "ProductorZona4";
    NomCont = "Contador";
    NombreArch  = "Archivo";
    
    //Inicialización de Semaforos
    ConsZona1 = CrearSemaforos(NomCons1, 0);
    ProdZona1 = CrearSemaforos(NomProd1, 1);
    ConsZona2 = CrearSemaforos(NomCons2, 0);
    ProdZona2 = CrearSemaforos(NomProd2, 1);
    ConsZona3 = CrearSemaforos(NomCons3, 0);
    ProdZona3 = CrearSemaforos(NomProd3, 1);
    ConsZona4 = CrearSemaforos(NomCons4, 0);
    ProdZona4 = CrearSemaforos(NomProd4, 1);
    Cont     = CrearSemaforos(NomCont, 1);
    Archivo      = CrearSemaforos(NombreArch, 1);
    
    //Toma el bloque de memoria compartida
    Memoria = AdjuntarMemoria();
    if(Memoria == NULL)
    {
        printf("No se pudo obtener el bloque de memoria\n");
        return -1;
    }
    
    //Zona de Productores
    for(x = 0; x < PRODUCTORES; x++)
    {
        pid = fork();
        
        if(pid == -1)
            perror("No se pudo crear el proceso hijo\n");
        else if(pid == 0)
        {
            //Toma el bloque de memoria compartida
            Memoria = AdjuntarMemoria();
            if(Memoria == NULL)
            {
            printf("No se pudo obtener el bloque de memoria\n");
            return -1;
            }
            
            //Inicia ciclo de producción
            for(y = 0; y < RANGO; y++)
            {
                while(1)
                {
                    //Zona Critica 1
                    sem_getvalue(ProdZona1, &flagZona1);
                    if(flagZona1 == 1)
                    {
                        sem_wait(ProdZona1);
                        Memoria[0] = (x+1) * 111;
                        printf("Productor No. %d Producto: %d Zona 1\n", x+1, Memoria[0]);
                        sem_post(ConsZona1);
                        break;
                    }
                    //Zona Critica 2
                    sem_getvalue(ProdZona2, &flagZona2);
                    if(flagZona2 == 1)
                    {
                        sem_wait(ProdZona2);
                        Memoria[1] = (x+1) * 111;
                        printf("Productor No. %d Producto: %d Zona 2\n", x+1, Memoria[1]);
                        sem_post(ConsZona2);
                        break;
                    }
                    //Zona Critica 3
                    sem_getvalue(ProdZona3, &flagZona3);
                    if(flagZona3 == 1)
                    {
                        sem_wait(ProdZona3);
                        Memoria[2] = (x+1) * 111;
                        printf("Productor No. %d Producto: %d Zona 3\n", x+1, Memoria[2]);
                        sem_post(ConsZona3);
                        break;
                    }
                    //Zona Critica 4
                    sem_getvalue(ProdZona4, &flagZona4);
                    if(flagZona4 == 1)
                    {
                        sem_wait(ProdZona4);
                        Memoria[3] = (x+1) * 111;
                        printf("Productor No. %d Producto: %d Zona 4\n", x+1, Memoria[3]);
                        sem_post(ConsZona4);
                        break;
                    }
                }
            }
            LimpiarSemaforos(NomCons1, ConsZona1);
            LimpiarSemaforos(NomProd1, ProdZona1);
            LimpiarSemaforos(NomCons2, ConsZona2);
            LimpiarSemaforos(NomProd2, ProdZona2);
            LimpiarSemaforos(NomCons3, ConsZona3);
            LimpiarSemaforos(NomProd3, ProdZona3);
            LimpiarSemaforos(NomCons4, ConsZona4);
            LimpiarSemaforos(NomProd4, ProdZona4);
            
            Des_Memo(Memoria);
            
            exit(0);
        }
    }
    
    //Zona de Consumo
    for(x = 0; x < CONSUMIDORES; x++)
    {
        pid = fork();
        if(pid == -1)
            perror("No se pudo crear el archivo hijo\n");
        else if(pid == 0)
        {
            while(1)
            {
                //Zona critica Contador
                sem_wait(Cont);
                //Verifica que no se haya consumido mas de la producción total
                if(Memoria[4] < RANGO * PRODUCTORES)
                {
                    //Verifica si estamos en el ultimo elemento
                    if(Memoria[4] == ((RANGO * PRODUCTORES)-1))
                        last = 1;
                    Memoria[4]++;
                    printf("%d\n", Memoria[4]);
                    sem_post(Cont);
                }else
                {
                    printf("FIN\n");
                    sem_post(Cont);
                    break;
                }
                
                while(1)
                {
                    //Zona Critica 1
                    sem_getvalue(ConsZona1, &flagZona1);
                    if(flagZona1 == 1)
                    {
                        sem_wait(ConsZona1);
                        printf("Consumidor No. %d Consumido: %d Zona 1\n", x+1, Memoria[0]);
                        GuardarProd(Archivo, Memoria[0], x+1, Memoria[4], doc);
                        sem_post(ProdZona1);
                        break;
                    }
                    //Zona Critica 2
                    sem_getvalue(ConsZona2, &flagZona2);
                    if(flagZona2 == 1)
                    {
                        sem_wait(ConsZona2);
                        printf("Consumidor No. %d Consumido: %d Zona 2\n", x+1, Memoria[1]);
                        GuardarProd(Archivo, Memoria[1], x+1, Memoria[4], doc);
                        sem_post(ProdZona2);
                        break;
                    }
                    //Zona Critica 3
                    sem_getvalue(ConsZona3, &flagZona3);
                    if(flagZona3 == 1)
                    {
                        sem_wait(ConsZona3);
                        printf("Consumidor No. %d Consumido: %d Zona 3\n", x+1, Memoria[2]);
                        GuardarProd(Archivo, Memoria[2], x+1, Memoria[4], doc);
                        sem_post(ProdZona3);
                        break;
                    }
                    //Zona Critica 4
                    sem_getvalue(ConsZona4, &flagZona4);
                    if(flagZona4 == 1)
                    {
                        sem_wait(ConsZona4);
                        printf("Consumidor No. %d Consumido: %d Zona 4\n", x+1, Memoria[3]);
                        GuardarProd(Archivo, Memoria[3], x+1, Memoria[4], doc);
                        sem_post(ProdZona4);
                        break;
                    }
                }
            }

            sem_unlink(NomCons1);
            sem_close(ConsZona1);
            sem_unlink(NomProd1);
            sem_close(ProdZona1);
            sem_unlink(NomCons2);
            sem_close(ConsZona2);
            sem_unlink(NomProd2);
            sem_close(ProdZona2);
            sem_unlink(NomCons3);
            sem_close(ConsZona3);
            sem_unlink(NomProd3);
            sem_close(ProdZona3);
            sem_unlink(NomCons4);
            sem_close(ConsZona4);
            sem_unlink(NomProd4);
            sem_close(ProdZona4);
            sem_unlink(NombreArch);
            sem_close(Archivo);
            sem_unlink(NomCont);
            sem_close(Cont);
            
            Des_Memo(Memoria);
            
            if(last)
            {
                sem_destroy(ConsZona1);
                sem_destroy(ProdZona1);
                sem_destroy(ConsZona2);
                sem_destroy(ProdZona2);
                sem_destroy(ConsZona3);
                sem_destroy(ProdZona3);
                sem_destroy(ConsZona4);
                sem_destroy(ProdZona4);
                sem_destroy(Cont);
                sem_destroy(Archivo);
                DestruirMemoria();
            }
        }
    }
    return 0;
}

int BloqueCompartido()
{
    key_t aux, key;
    
    
    key = ftok("/bin/ls", 0);
    
    if(key == IPC_RESULT_ERROR)
        return IPC_RESULT_ERROR;
    
    
    return shmget(key, sizeof(int)*5, 0644|IPC_CREAT);
}

int * AdjuntarMemoria()
{
    int sharedBlockID = BloqueCompartido();
    int *result;
    
    if(sharedBlockID == IPC_RESULT_ERROR)
        return NULL;
    
    
    result = shmat(sharedBlockID, NULL, 0);
    if(result == NULL)
        return NULL;
    
    return result;
}

int Des_Memo(int *block)
{
    return (shmdt(block) != IPC_RESULT_ERROR);
    printf("OK");
}

int DestruirMemoria()
{
    int sharedBlockID = BloqueCompartido();
    
    if(sharedBlockID == IPC_RESULT_ERROR)
        return IPC_RESULT_ERROR;
    
    return(shmctl(sharedBlockID, IPC_RMID, NULL) != IPC_RESULT_ERROR);
}

sem_t * CrearSemaforos (char * name, int init)
{
	sem_t * auxS;

	if((auxS = sem_open(name, O_CREAT|O_EXCL, 0666, init)) == SEM_FAILED)
	{
		if((auxS = sem_open(name, 0)) == SEM_FAILED)
			perror("Error al abrir semaforo\n");
		else
			printf("Semaforo ligado de manera correcta\n");
	}else
	{
		printf("Semaforo con nombre %s creado de manera correcta\n", name);
		sem_init(auxS, 1, init);
	}

	return auxS;
}

void LimpiarSemaforos(char *name, sem_t * semaphore)
{
    sem_unlink(name);
    sem_close(semaphore);
}

void GuardarProd(sem_t *Archivo, int prod, int cons, int zone, FILE *doc)
{
    sem_wait(Archivo);
    
    doc = fopen("PF.txt", "a");
    
    if(doc == NULL)
        perror("No se pudo crear el archivo\n");
    
    fprintf(doc, ".- %d Consumidor:%d en Zona :%d\n", prod, cons, zone);
    
    
    fclose(doc);
    sem_post(Archivo);
}
