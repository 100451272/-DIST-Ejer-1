// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "claves.h"

#include <pthread.h>

// declare a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void petition_handler(mqd_t queue){
    struct peticion pet;
    int result;

    while(1) {
        if (mq_receive(queue, (char *)&pet, sizeof(pet), 0) == -1){
            printf("Error en el recieve");
            continue;
        }

        // acquire the mutex before accessing the data structures
        pthread_mutex_lock(&mutex);

        switch (pet.op) {
            case INIT:
                if(pet.delete_all) {
                    deleteAll();
                    result = 0;
                } else {
                    result = -1; // Error: missing delete_all flag
                }
                break;

            case SET_VALUE:
                if(exist(pet.clave)) {
                    modify_value(pet.clave, pet.valor1, pet.valor2, pet.valor3);
                    result = 0;
                } else {
                    set_value(pet.clave, pet.valor1, pet.valor2, pet.valor3);
                    result = 1;
                }
                break;

            case GET_VALUE:
                if(exist(pet.clave)) {
                    result = get_value(pet.clave, pet.valor1, pet.valor2, pet.valor3);
                } else {
                    result = -1; // Error: key not found
                }
                break;

            case DELETE_KEY:
                if(exist(pet.clave)) {
                    deleteKey(pet.clave);
                    result = 0;
                } else {
                    result = -1; // Error: key not found
                }
                break;

            case EXIST:
                result = exist(pet.clave);
                break;

            case COPY_KEY:
                if(exist(pet.clave)) {
                    copy_key(pet.clave, pet.valor1);
                    result = 0;
                } else {
                    result = -1; // Error: key not found
                }
                break;

            default:
                result = -1; // Error: unknown operation
        }

        // release the mutex after accessing the data structures
        pthread_mutex_unlock(&mutex);

        if(mq_send(queue, (char *)&result, sizeof(result), 0) == -1) {
            printf("Error en el send");
        }
    }
}



int main(void){
    mqd_t queue;
    queue = mq_open("/ALMACEN", O_CREAT);

    if (queue == -1){
        printf("Error");
        return -1;
    }


    printf("Cola creada\n");
    petition_handler(queue);
    mq_close(queue);
    // Remove the message queue from the system
    mq_unlink("/ALMACEN");
    return 0;
}
