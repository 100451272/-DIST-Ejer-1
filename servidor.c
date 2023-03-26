
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
            delete(&list, pet.tupla.clave);
            set(&list, pet.tupla.clave, &pet.tupla);
            printList(list);
            break;



            /* case 3: //MODIFY_VALUE
                 pthread_mutex_lock(&mutex); // bloqueamos el mutex para que no haya conflictos de acceso a la lista
                 // verificamos si la clave existe
                 if (exists(&list, pet.tupla.clave)) {
                     // obtenemos la tupla original
                     struct tupla original;
                     get(&list, pet.tupla.clave, &original);

                     // copiamos los nuevos valores a la tupla original
                     strcpy(original.valor1, pet.tupla.valor1);
                     original.valor2 = pet.tupla.valor2;
                     original.valor3 = pet.tupla.valor3;

                     // actualizamos la tupla en la lista
                     set(&list, pet.tupla.clave, &original);

                     res.op = 0; // operación exitosa
                 } else {
                     res.op = -1; // error: la clave no existe
                 }
                 printList(list);
                 //delete(&list, pet.tupla.clave);
                 pthread_mutex_unlock(&mutex);
                 printList(list);
                 break;*/



        case 4: //DELETE_KEY
            if (delete(&list, pet.tupla.clave) == 0) {
                res.op = 0;
            } else {
                res.op = -1;
            }
            printList(list);
            break;



        case 5: //EXIST
            res.op = exists(&list, pet.tupla.clave);
            if (res.op == 0) {
                printf("No existe\n");
            }
            else if (res.op == 1){
                printf("Existe\n");
            }
            printList(list);
            break;


        case 6: // COPY_KEY
        {
            struct tupla tupla1, tupla2;
            int key1 = pet.tupla.clave;
            int key2 = pet.tupla.valor2;

            // Buscar la tupla de key1
            if (get(&list, key1, &tupla1) == -1) {
                res.op = -1; // La clave key1 no existe
                break;
            }

            // Crear la tupla de key2 con los valores de key1
            tupla2.clave = key2;
            strcpy(tupla2.valor1, tupla1.valor1);
            tupla2.valor2 = tupla1.valor2;
            tupla2.valor3 = tupla1.valor3;

            // Insertar o modificar la tupla en la lista
            delete(&list, tupla2.clave);
            set(&list, key2, &tupla2);

            res.op = 0;
            printList(list);
            break;
        }






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
