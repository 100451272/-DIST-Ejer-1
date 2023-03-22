#ifndef P_LISTA_H
#define P_LISTA_H

struct node{
    struct node *prev;
    struct node *next;
    struct tupla *tupla;

};

struct tupla {
    int clave;
    char valor1[255];
    int valor2;
    double valor3;
};

struct peticion{
    int op;
    struct tupla tupla;
};

void printTupla(void);

void init(void);

void append(struct tupla tupla);

int isEmpty(void);

int deleteKey(int clave);

int deleteAll(void);

int exist(int clave);

int selectTupla(int clave, struct tupla *tupla);

int saveTupla(struct tupla *tupla);

#endif //P_LISTA_H