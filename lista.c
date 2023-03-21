#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"



struct node *head = NULL;
struct node *tail = NULL;

void init(void) {
    head = NULL;
    tail = NULL;
}

void append(struct tupla *tupla) {
    struct node *node = (struct node*) malloc(sizeof(struct node));

    node->tupla = tupla;
    node->prev = NULL;
    node->next = NULL;

    if (head == NULL) {
        head = node;
        tail = node;
    } else {
        node->next = head;
        head->prev = node;
        head = node;
    }
}

int isEmpty(void) {
    if (head == NULL){
        return 1;
    }
    return 0;
}

int deleteKey(int clave) {
    if (isEmpty()) {
        return -1;
    }
    struct node *current = head;
    while (current != NULL) {
        if (current->tupla->clave == clave) {
            if (current == head) {
                head = current->next;
            }
            if (current == tail) {
                tail = current->prev;
            }
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            free(current->tupla);
            free(current);
            return 0;
        }
        current = current->next;
    }
    return -1;
}
int deleteAll(void) {
    if (isEmpty()) {
        return 0;
    }
    struct node *current = head;
    while (current != NULL) {
        struct node *next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    tail = NULL;
    return 0;
}

int exist(int clave) {
    struct node *current = head;
    while (current != NULL) {
        if (current->tupla->clave == clave) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

int selectTupla(int clave, struct tupla *tupla) {
    struct node *current = head;
    while (current != NULL) {
        if (current->tupla->clave == clave) {
            *tupla = *(current->tupla);
            return 0;
        }
        current = current->next;
    }
    return -1;
}

int saveTupla(struct tupla *tupla) {
    struct node *current = head;
    while (current != NULL) {
        if (current->tupla->clave == tupla->clave) {
            free(current->tupla->valor1);
            current->tupla->valor1 = strdup(tupla->valor1);
            current->tupla->valor2 = tupla->valor2;
            current->tupla->valor3 = tupla->valor3;
            return 0;
        }
        current = current->next;
    }
    return -1;
}