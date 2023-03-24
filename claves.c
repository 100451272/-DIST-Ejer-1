
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"
#include <string.h>
#include <mqueue.h>

int init(void){
    mqd_t q_servidor;
    mqd_t q_cliente;

    struct peticion pet;
    struct peticion res;
    struct mq_attr attr;

    attr.mq_maxmsg = 10;     attr.mq_msgsize = sizeof(res);
    q_cliente = mq_open("/CLIENTE", O_CREAT|O_RDONLY, 0700, &attr);
	if (q_cliente == -1) {
		perror("mq_open");
		return -1;
	}
    q_servidor = mq_open("/ALMACEN", O_WRONLY);
	if (q_servidor == -1){
		mq_close(q_cliente);
		perror("mq_open");
		return -1;
	}
    /* se rellena la petición */
    pet.op = 0; 
	    

    if (mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0) < 0){
		perror("mq_send");
		return -1;
    }	
    if (mq_receive(q_cliente, (char *)&res, sizeof(res), 0) < 0){
		perror("mq_recv");
		return -1;
    }	

    printf("Resultado = %d\n", res.op);

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink("/CLIENTE");
	return res.op;
}


int set_value(int key, char *value1, int value2, double value3){
    mqd_t q_servidor;
    mqd_t q_cliente;

    struct peticion pet;
    struct peticion res;
    struct mq_attr attr;

    attr.mq_maxmsg = 1;     attr.mq_msgsize = sizeof(res);
    q_cliente = mq_open("/CLIENTE", O_CREAT|O_RDONLY, 0700, &attr);
	if (q_cliente == -1) {
		perror("mq_open");
		return -1;
	}
    q_servidor = mq_open("/ALMACEN", O_WRONLY);
	if (q_servidor == -1){
		mq_close(q_cliente);
		perror("mq_open");
		return -1;
	}
    /* se rellena la petición */
    struct tupla tupla;
    tupla.clave = key;
    strcpy(tupla.valor1, value1);
    tupla.valor2 = value2;
    tupla.valor3 = value3;

    pet.op = 1;
    pet.tupla = tupla;

    if (mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0) < 0){
		perror("mq_send");
		return -1;
    }	
    if (mq_receive(q_cliente, (char *) &res, sizeof(res), 0) < 0){
		perror("mq_recv");
		return -1;
    }	

    printf("Resultado = %d\n", res.op);

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink("/CLIENTE");
	return res.op;    
}

int get_value(int key, char *value1, int *value2, double *value3){
    mqd_t q_servidor;
    mqd_t q_cliente;

    struct peticion pet;
    struct peticion res;
    struct mq_attr attr;

    attr.mq_maxmsg = 10;     attr.mq_msgsize = sizeof(res);
    q_cliente = mq_open("/CLIENTE", O_CREAT|O_RDONLY, 0700, &attr);
	if (q_cliente == -1) {
		perror("mq_open");
		return -1;
	}
    q_servidor = mq_open("/ALMACEN", O_WRONLY);
	if (q_servidor == -1){
		mq_close(q_cliente);
		perror("mq_open");
		return -1;
	}
    /* se rellena la petición */
    pet.op = 2;
	    
    if (mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0) < 0){
		perror("mq_send");
		return -1;
    }	
    if (mq_receive(q_cliente, (char *) &res, sizeof(res), 0) < 0){
		perror("mq_recv");
		return -1;
    }	
    printf("Resultado = %d\n", res.op);
    
    value1 = res.tupla.valor1;
    *value2 = res.tupla.valor2;
    *value3 = res.tupla.valor3;

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink("/CLIENTE");
    return 0;
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
    strcpy(tupla.valor1, value1);
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
    if (res != 0 && res != 1) {
        perror("Respuesta inválida recibida");
        return -1;
    }
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
