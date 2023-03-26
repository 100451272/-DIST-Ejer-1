
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

int modify(int key1, int key2) {
    mqd_t q_servidor;
    mqd_t q_cliente;
    struct peticion res;
    struct mq_attr attr;
    int res_value;
    char value1[2048];
    int value2;
    double value3;

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(res);
    q_cliente = mq_open("/CLIENTE", O_CREAT | O_RDONLY, 0700, &attr);
    if (q_cliente == -1) {
        perror("mq_open");
        return -1;
    }
    q_servidor = mq_open("/ALMACEN", O_WRONLY);
    if (q_servidor == -1) {
        mq_close(q_cliente);
        perror("mq_open");
        return -1;
    }
    // Check if key1 exists
    res_value = exist(key1);
    if (res_value == 0) {
        printf("La clave %d no existe\n", key1);
        return -1;
    } else if (res_value == -1) {
        perror("Error en las comunicaciones");
        return -1;
    }

    // Copy values from key1 to key2
    res_value = get_value(key1, value1, &value2, &value3);
    if (res_value == -1) {
        perror("Error en las comunicaciones");
        return -1;
    }

    res.op = 1;
    res.tupla.clave = key2;
    strncpy(res.tupla.valor1, value1, 2048);
    res.tupla.valor2 = value2;
    res.tupla.valor3 = value3;

    if (mq_send(q_servidor, (const char *) &res, sizeof(res), 0) < 0) {
        perror("mq_send");
        return -1;
    }

    if (mq_receive(q_cliente, (char *) &res, sizeof(res), 0) < 0) {
        perror("mq_recv");
        return -1;
    }

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink("/CLIENTE");

    if (res.op == 0) {
        printf("La clave %d ya existe\n", key2);
        return -1;
    } else if (res.op == 1) {
        printf("La clave %d ha sido creada y copiada exitosamente\n", key2);
        return 0;
    } else if (res.op == 2) {
        printf("La clave %d ha sido modificada exitosamente\n", key2);
        return 0;
    } else {
        perror("Error en las comunicaciones");
        return -1;
    }
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
    pet.tupla.clave = key;
	    
    if (mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0) < 0){
		perror("mq_send");
		return -1;
    }	
    if (mq_receive(q_cliente, (char *) &res, sizeof(res), 0) < 0){
		perror("mq_recv");
		return -1;
    }	
    printf("Resultado = %d\n", res.op);
    
    strcpy(value1, res.tupla.valor1);
    *value2 = res.tupla.valor2;
    *value3 = res.tupla.valor3;

    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink("/CLIENTE");
    return res.op;
}

int modify_value(int key, char *value1, int value2, double value3){
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
    pet.op = 3;
    pet.tupla.clave = key;
    strcpy(pet.tupla.valor1, value1);
    pet.tupla.valor2 = value2;
    pet.tupla.valor3 = value3;
	    
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


int delete_key(int key){
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
    pet.op = 4;
    pet.tupla.clave = key;
	    
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

int exist(int key){
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
    pet.op = 5;
    pet.tupla.clave = key;
	    
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

int copy_key(int key1, int key2){
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
    pet.tupla.clave = key1;
    pet.tupla.valor2 = key2; // Reutilizamos el valor 2 como paso de la clave 2
    pet.op = 6;
	    
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

