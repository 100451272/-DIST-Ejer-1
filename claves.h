//
// Created by manu on 15/03/23.
//

#ifndef DIST_EJER_1_CLAVES_H
#define DIST_EJER_1_CLAVES_H
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
    struct node *prev;
    struct node *next;
    struct tupla *tupla;

};

void append(struct tupla *tupla);

int isEmpty(void);

int deleteKey(int clave);

int deleteAll(void);

int exist(int clave);

int selectTupla(int clave, struct tupla *tupla);

int saveTupla(struct tupla *tupla);

int set_value(int key, char *value1, int value2, double value3)


#endif //DIST_EJER_1_CLAVES_H
