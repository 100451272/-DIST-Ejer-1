// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "lista.h"

#include <pthread.h>

// declare a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct peticion petition_handler(struct peticion pet){
    struct tupla tupla;
    struct peticion res;
    switch (pet.op) {
        case 0: //INIT
            if (!isEmpty()){
                deleteAll();
                init();
                res.op = 0;
            }
            res.op= 0;
            break;

        case 1: //SET_VALUE
            append(pet.tupla);
            res.op = 0;
            break;

        case 2: //GET_VALUE
            res.op = selectTupla(pet.tupla.clave, &res.tupla);
            break;

        case 3: //MODIFY_VALUE
            /*
            if (!exist(pet.tupla.clave)){
                break;
            }
            res.op = saveTupla(&pet.tupla);*/
            printTupla();
            break;

        case 4: //DELETE_KEY
            res.op = deleteKey(pet.tupla.clave);
            break;

        case 5: //EXIST
            res.op = exist(pet.tupla.clave);
            break;

        case 6: //COPY_KEY
            res.op = selectTupla(pet.tupla.clave, &tupla);
            int key2 = pet.tupla.valor2;
            if (res.op == -1){
                break;
            }
            tupla.clave = key2;
            res.op = saveTupla(&tupla);
            break;
        default:
            res.op = -1; // Error: unknown operation
    }
    return res;
}



int main(void){
    mqd_t q_servidor;     	    /* cola de mensajes del servidor */
    mqd_t q_cliente;                /* cola de mensajes del cliente */
    struct peticion pet;
    struct mq_attr attr;

    attr.mq_maxmsg = 10;                
	attr.mq_msgsize = sizeof(struct peticion);
    q_servidor = mq_open("/ALMACEN", O_CREAT|O_RDONLY, 0700, &attr);
	if (q_servidor == -1) {
		perror("mq_open");
		return -1;
	}

        while(1) {
        if (mq_receive(q_servidor, (char *) &pet, sizeof(pet), 0) < 0){
			perror("mq_recev");
			return -1;
		}
        printf("Peticion recibida: %d\n", pet.op);
        struct peticion res = petition_handler(pet);
        //printf("%d", res.op);
                /* se responde al cliente abriendo reviamente su cola */
        q_cliente = mq_open("/CLIENTE", O_WRONLY);
		if (q_cliente < 0) {
			perror("mq_open");
			mq_close(q_servidor);
			mq_unlink("/ALMACEN");
			return -1;
		}
        
        //printf("%s", res.tupla.valor1);
        if (mq_send(q_cliente, (const char *)&res, sizeof(res), 0) < 0) {
			perror("mq_send");
			mq_close(q_servidor);
			mq_unlink("/ALMACEN");
            mq_close(q_cliente);
			return -1;
		}
        
                mq_close(q_cliente);
        }   
    mq_close(q_servidor);
    // Remove the message queue from the system
    mq_unlink("/ALMACEN");
    return 0;
}
