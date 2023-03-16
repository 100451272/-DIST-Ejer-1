//
// Created by manu on 15/03/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "claves.h"

struct node *head = NULL;

struct node *current = NULL;

void append(struct tupla *tupla){
    struct node *node = (struct node*) malloc(sizeof(struct node));

    node->tupla = tupla;
    node->next = head;
    head->prev = node;
    node->prev = NULL;
    head = node;
}

int isEmpty(void){
    if (head == NULL){
        return 1;
    }
    return 0;
}

int deleteKey(int clave){
    if (isEmpty()){
        return -1;
    }
    current = head;
    while (current != NULL){
        if (current->tupla->clave == clave){
            struct node *next = current->next;
            struct node *prev = current->prev;
            next->prev = prev;
            prev->next = next;
            free(current);
            return 0;
        }
        current = current->next;
    }
    return -1;
}

int deleteAll(void){
    if (isEmpty()){
        return 0;
    }
    current = head;
    while (current != NULL){
        struct node *next = current->next;
        free(current);
        current = next;
    }
    if (current == NULL){
        return 0;
    }
    return -1;
}

int exist(int clave){
    if (isEmpty()){
        return -1;
    }
    current = head;
    while (current != NULL){
        if (current->tupla->clave == clave){
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int selectTupla(int clave, struct tupla *tupla){
    if (!exist(clave)){
        return -1;
    }
    current = head;
    while(current != NULL){
        if (current->tupla->clave == clave){
            tupla = current->tupla;
            return 0;
        }
        current = current->next;
    }
    return -1;
}

int saveTupla(struct tupla *tupla){
    if (!exist(tupla->clave)){
        return -1;
    }
    current = head;
    while (current != NULL){
        if (current->tupla->clave == tupla->clave){
            current->tupla = tupla;
            return 0;
        }
        current = current->next;
    }
    return -1;
}
// Codigo para manejar las tuplas y llamar desde el servidor

int set_value(int key, char *value1, int value2, double value3){
    struct tupla tupla;
    tupla.clave = key;
    tupla.valor1 = value1;
    tupla.valor2 = value2;
    tupla.valor3 = value3;

    append(&tupla);
    return 0;
}

int get_value(int key, char *value1, int *value2, double *value3){
    struct tupla tupla;
    int res = selectTupla(key, &tupla);
    value1 = tupla.valor1;
    value2 = tupla.valor2;

    return res;
}

int modify_value(int key, char *value1, int value2, double value3){
    struct tupla tupla;
    if (!exist(key)){
        return -1;
    }
    int res = saveTupla(&tupla);
    return res;
}

int delete_key(int key){
    int res = deleteKey(key);
    return res;
}

int copy_key(int key1, int key2){
    struct tupla tupla;
    int res1 = selectTupla(key1, &tupla);
    if (res1 == -1){
        return -1;
    }
    tupla.clave = key2;
    int res2 = saveTupla(&tupla);
    if (res2 == -1){
        return -1;
    }
    return 0;
}
