// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "claves.h"




void petition_handler(mqd_t queue){
    //printf("llamo a la funcion\n");
    struct peticion pet;

    if (mq_receive(queue, (char *)&pet, sizeof(pet), 0) == -1){
        printf("error en el recieve");
        return;
    }
    printf("%s", pet.valor1);
    switch (pet.op) {
        case 0: //INIT
            deleteAll();
        case 1: //set_value
            setValue(pet.clave, pet.valor1, pet.valor2, pet.valor3);

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
    return 0;
}
