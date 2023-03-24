#ifndef _LISTA_H
#define _LISTA_H        1

#define MAX_KEY_LENGTH	256

struct Node{ 
	int 	key;
	struct  tupla *tupla;
	struct 	Node *next; 
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

typedef struct Node * List;


int init(List *l);
int set(List *l, int key, struct tupla *tupla);
int get(List *l, int key, struct tupla *tupla);
int printList(List l);
int delete(List *l, int key);
int destroy(List *l);

#endif

