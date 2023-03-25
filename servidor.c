
// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include "list.h"

#include <pthread.h>

// declare a global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
List list;

struct peticion petition_handler(struct peticion pet){
    //struct tupla tupla;
    struct peticion res;
    switch (pet.op) {
        case 0: //INIT
            init(&list);
            res.op= 0;
            break;

        case 1: //SET_VALUE
            set(&list, pet.tupla.clave, &pet.tupla);
            res.op = 0;
            printList(list);
            break;

        case 2: //GET_VALUE
            res.op = get(&list, pet.tupla.clave, &pet.tupla);
            if (res.op == 0) { // si la operación fue exitosa
                // copiamos los valores de la tupla a los campos de la respuesta
                strcpy(res.tupla.valor1, pet.tupla.valor1);
                res.tupla.valor2 = pet.tupla.valor2;
                res.tupla.valor3 = pet.tupla.valor3;
            }
            break;



        case 3: //MODIFY_VALUE
            int modif_res = -1;
            struct tupla old_tupla;
            if (get(&list, pet.tupla.clave, &old_tupla) == 0) {
                // Si la clave existe, se actualiza la tupla
                delete(&list, pet.tupla.clave);
                modif_res = set(&list, pet.tupla.clave, &pet.tupla);
                if (modif_res == 0) {
                    printf("Tupla actualizada:\n");
                    printf("  clave: %d\n", pet.tupla.clave);
                    printf("  valor1: %s\n", pet.tupla.valor1);
                    printf("  valor2: %d\n", pet.tupla.valor2);
                    printf("  valor3: %lf\n", pet.tupla.valor3);
                }
            }
            res.op = 0;
            break;

        case 4: //DELETE_KEY
            if (delete(&list, pet.tupla.clave) == 0) {
                res.op = 0;
            } else {
                res.op = -1;
            }
            printList(list);
            break;



        case 5: // EXISTS
            if (get(&list, pet.tupla.clave, &pet.tupla) == 0) {
                res.op = 1; // found
            } else {
                res.op = -1; // not found
            }
            break;



            /* case 6: //COPY_KEY
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
         */}
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
        fflush(NULL);
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
