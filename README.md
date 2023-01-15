# Zut_projekt_burak
Projekt na ZUT (Shm/Semafore)

# Plik main:

Funkcje:

to jest tworzona pamięć dzielona (shm_id)

```C
    shm_id = shmget(0x0316316, (SIZE+1)*sizeof(char), IPC_CREAT|0600);
    if (shm_id == -1){perror("ERROR creating shared memory segment");exit(1);}
```

```C
shmget(<Wartość_Klucza_do_pamięci>, <8 bitów * ilość_znaków + 1 zna_ kończący>, <łączy_Uprawnienia_do_zapisu_i_odczytu>);
```

```C
    shared = (char*)shmat(shm_id, NULL, 0);
    if (shared == NULL){perror("ERROR attaching shared memory segment");exit(1);}
```
```C
    //semafor
    sem_unlink(sem_fn);
    sem_des = sem_open(sem_fn, O_CREAT, 0644, 0);
    if(sem_des == (void*)-1){perror("sem_open failure");_exit(-1);}
    sem_post(sem_des);
```


```mermaid
graph TD;
    P1(Main)-->P2(Sensor A);
    P1(Main)-->P3(Sensor B);
    P1(Main)-->P4(Sensor C);
```
