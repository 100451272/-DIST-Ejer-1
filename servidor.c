// Código del servidor
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

struct tupla {
    int clave;
    char* valor1;
    int valor2;
    double valor3;
};

struct peticion{
    int op;
    int clave;
    char* valor1;
    int valor2;
    double valor3;
    int clave2;
};

struct node{
    int *prev;
    int *next;
    int *tupla;

};

const int *startTuple = malloc(sizeof(struct node));

void init_node(int *tupla){
    int *last = searchLast() //Apuntar a NULL
}

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
