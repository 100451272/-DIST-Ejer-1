// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "lista.h"

#include <pthread.h>

// declare a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
mqd_t queue;

void petition_handler(struct peticion pet){
    int res;
    printf("%d", pet.op);
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

    mqd_t cola_cliente;
    cola_cliente = mq_open("/CLIENTE", O_WRONLY);
    if(mq_send(cola_cliente, (char *)&res, sizeof(int), 0) == -1) {
        printf("Error en el send");
    }
}



int main(void){
    struct peticion pet;     
	//pthread_attr_t t_attr;		 atributos de los threads 
   	//pthread_t thid;


    queue = mq_open("/ALMACEN", O_CREAT|O_RDONLY, 0700);
	if (queue == -1) {
		perror("mq_open");
		return -1;
	}

    
    printf("Cola creada\n");

    while (1){
        if (mq_receive(queue, (char *)&pet, sizeof(pet), 0) == -1){
        printf("Error en el recieve");
        return -1;
        }
        printf("Recibido");
        petition_handler(pet);
    }    
    mq_close(queue);
    // Remove the message queue from the system
    mq_unlink("/ALMACEN");
    return 0;
}
