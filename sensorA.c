#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/fcntl.h>

#define SIZE 90

char sem_fn[] = "my_sem";
sem_t *sem_des;


int main(int argc, char *argv[])
{
    int shm_id; 
    char *shared; 
    //pamięś dzielona
    shm_id = shmget(0x0316316, (SIZE+1)*sizeof(char), IPC_CREAT|0600);
    if (shm_id == -1){perror("ERROR creating shared memory segment");exit(1);}

    shared = (char*)shmat(shm_id, NULL, 0);
    if (shared == NULL){perror("ERROR attaching shared memory segment");exit(1);}

    //semafor
    sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);
    if(sem_des == (void*)-1){perror("sem_open failure");_exit(-1);}


    int pid = getpid();
    int val;
    while (1)
    {
        printf("-----------Sensor A----------PID:\t pid: %d\n",pid);
        printf("podaj wartość: ");
        scanf("%d",&val);
        if(val>=120)val=120;
        if(val<=0)val=0;
        sem_wait(sem_des);
        shared[100]=val;
        sem_post(sem_des);
        printf("Wartość zapisana: %d\n",val);
    }
    
    
    return 0;
}

    