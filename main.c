#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#define SIZE 90

char sem_fn[] = "my_sem";
sem_t *sem_des;
int shm_id; 
char *shared; 



int main(int argc, char *argv[])
{
    
    //pamięś dzielona
    shm_id = shmget(0x0316316, (SIZE+1)*sizeof(char), IPC_CREAT|0600);
    if (shm_id == -1){perror("ERROR creating shared memory segment");exit(1);}

    shared = (char*)shmat(shm_id, NULL, 0);
    if (shared == NULL){perror("ERROR attaching shared memory segment");exit(1);}

    //semafor
    sem_unlink(sem_fn);
    sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);
    if(sem_des == (void*)-1){perror("sem_open failure");_exit(-1);}
    sem_post(sem_des);

    int sensA=0;
    int sensB=0;
    int sensC=0;
    int sens_binary_A=0;
    int sens_binary_B=0;
    int sens_binary_C=0;
    int val_sensA=0;
    int val_sensB=0;
    int val_sensC=0;


    sem_wait(sem_des);
    shared[100]=0;
    shared[200]=0;
    shared[300]=0;
    sem_post(sem_des);




    printf("podaj pid sensora A: ");
    scanf("%d",&sensA);
    printf("podaj pid sensora B: ");
    scanf("%d",&sensB);
    printf("podaj pid sensora C: ");
    scanf("%d",&sensC);

    char *SA="OFF";
    char *SB="OFF";
    char *SC="OFF";
    int alarm = 0;

    while (1)
    {
        sem_wait(sem_des);
        val_sensA = shared[100];
        val_sensB = shared[200];
        val_sensC = shared[300];
        sem_post(sem_des);
        if (kill(sensA, 0) == 0 && sensA>0) sens_binary_A=1;
        if (kill(sensB, 0) == 0 && sensB>0) sens_binary_B=1;
        if (kill(sensC, 0) == 0 && sensC>0) sens_binary_C=1;

        //termostaty
        if(val_sensA<=20){SA="ON";}if(val_sensA>=23){SA="OFF";}
        if(val_sensB<=21){SB="ON";}if(val_sensB>=23){SB="OFF";}
        if(val_sensC<=20){SC="ON";}if(val_sensC>=24){SC="OFF";}
        
        alarm=0;
        if(val_sensA>=50 || val_sensB>=50 || val_sensC>=60){SA=SB=SC="ALARM";alarm=1;}else
        
        if(sens_binary_A==0){SA="ERROR";}
        if(sens_binary_B==0){SB="ERROR";}
        if(sens_binary_C==0){SC="ERROR";}

        printf("----status----\n");
        if(sens_binary_A==1){printf("\033[0;32m");}else{printf("\033[1;31m");}
        if(SA=="ALARM"){printf("\033[0;33m");}
        printf("sensorA: Status: %d \t val: %d \t grzanie %s \n",sens_binary_A,val_sensA,SA);
        
        if(sens_binary_B==1){printf("\033[0;32m");}else{printf("\033[1;31m");}
        if(SB=="ALARM"){printf("\033[0;33m");}
        printf("sensorB: Status: %d \t val: %d \t grzanie %s \n",sens_binary_B,val_sensB,SB);
        
        if(sens_binary_C==1){printf("\033[0;32m");}else{printf("\033[1;31m");}
        if(SC=="ALARM"){printf("\033[0;33m");}
        printf("sensorC: Status: %d \t val: %d \t grzanie %s \n",sens_binary_C,val_sensC,SC);
        sens_binary_A=0;
        sens_binary_B=0;
        sens_binary_C=0;
        
        sleep(1);
    }
    
    sem_unlink(sem_fn);
    return 0;
}

