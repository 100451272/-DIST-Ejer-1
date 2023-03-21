
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"
#include <string.h>
#include <mqueue.h>

int init(void){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct peticion pet;
    pet.op = 0;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}


int set_value(int key, char *value1, int value2, double value3){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key;
    tupla.valor1 = value1;
    tupla.valor2 = value2;
    tupla.valor3 = value3;
    struct peticion pet;
    pet.op = 1;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}

int get_value(int key, char **value1, int *value2, double *value3){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key;
    tupla.valor1 = NULL;
    tupla.valor2 = 0;
    tupla.valor3 = 0;

    struct peticion pet;
    pet.op = 2;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    
    *value1 = strdup(tupla.valor1);
    *value2 = tupla.valor2;
    *value3 = tupla.valor3;

    mq_close(cola);
    return res;
}

int modify_value(int key, char *value1, int value2, double value3){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key;
    tupla.valor1 = value1;
    tupla.valor2 = value2;
    tupla.valor3 = value3;
    struct peticion pet;
    pet.op = 3;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}


int delete_key(int key){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key;
    struct peticion pet;
    pet.op = 4;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}

int exist(int key){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key;
    struct peticion pet;
    pet.op = 5;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}

int copy_key(int key1, int key2){
    mqd_t cola;
    cola = mq_open("/ALMACEN", O_RDWR);
    if (cola == -1){
        perror("mq_open");
        return -1;
    }
    struct tupla tupla;
    tupla.clave = key1;
    tupla.valor2 = key2; // Reutilizamos el valor 2 como paso de la clave 2
    struct peticion pet;
    pet.op = 6;
    pet.tupla = tupla;
    if (mq_send(cola, (const char*)&pet, sizeof(struct peticion), 0) == -1){
        perror("mq_send");
        mq_close(cola);
        return -1;
    }
    int res;
    if (mq_receive(cola, (char*)&res, sizeof(int), 0) == -1){
        perror("mq_recieve");
        mq_close(cola);
        return -1;
    }
    mq_close(cola);
    return res;
}
