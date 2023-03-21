// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "lista.h"

#include <pthread.h>

// declare a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void petition_handler(mqd_t queue){
    struct peticion pet;
    int res;

    if (mq_receive(queue, (char *)&pet, sizeof(pet), 0) == -1){
        printf("Error en el recieve");
        return;
    }

    switch (pet.op) {
        case 0: //INIT
            if (!isEmpty()){
                deleteAll();
                init();
                res = 0;
            }
            break;

        case 1: //SET_VALUE
            append(&pet.tupla);
            res = 0;
            break;

        case 2: //GET_VALUE
            selectTupla(pet.tupla.clave, &pet.tupla);
            break;

        case 3: //MODIFY_VALUE
            if (!exist(pet.tupla.clave)){
                break;
            }
            res = saveTupla(&pet.tupla);
            break;

        case 4: //DELETE_KEY
            res = deleteKey(pet.tupla.clave);
            break;

        case 5: //EXIST
            res = exist(pet.tupla.clave);
            break;

        case 6: //COPY_KEY
            int key1 = pet.tupla.clave;
            int key2 = pet.tupla.valor2;
            struct tupla tupla;
            res = selectTupla(key1, &tupla);
            if (res == -1){
                break;
            }
            tupla.clave = key2;
            res = saveTupla(&tupla);
            break;
        default:
            res = -1; // Error: unknown operation
    }
/*
    if(mq_send(queue, (char *)&res, sizeof(int), 0) == -1) {
        printf("Error en el send");
    }*/
}



int main(void){
    mqd_t queue;
    queue = mq_open("/ALMACEN", O_CREAT | O_RDONLY);

    if (queue == -1){
        printf("Error");
        return -1;
    }


    printf("Cola creada\n");
    while (1){
    petition_handler(queue);
    }    
    mq_close(queue);
    // Remove the message queue from the system
    mq_unlink("/ALMACEN");
    return 0;
}
