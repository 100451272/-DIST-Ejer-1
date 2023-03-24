#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "list.h"


int init(List *l) {

	*l = NULL;
	return (0);
}	

int set(List *l, int key, struct tupla *tupla){
	struct Node *ptr;

	ptr = (struct Node *) malloc(sizeof(struct Node));
	if (ptr == NULL) 
		return -1;

	if (*l == NULL) {  // emtpy list
		ptr->key = key;
		ptr->tupla = &tupla;
		ptr->next = NULL;
		*l = ptr;
	}
	else {
		// insert in head
		ptr->key = key;
		ptr->tupla = &tupla;
		ptr->next = *l;
		*l = ptr;
	}


	return 0;
}	

int get(List *l, int key, struct tupla *tupla){
	List aux;

	aux = l;	

	while (aux!=NULL) {
		if (aux->key == key) {
			*tupla = *(aux->tupla);
			return 0;		// found
		}
		else
			aux = aux->next;
	}

	return -1;  // not found
}	

int printList(List l){
	List aux;

	aux = l;

	while(aux != NULL){
		printf("Key=%d    1=%s	  2=%d 3=%ld\n", aux->key, aux->tupla->valor1,
		aux->tupla->valor2, aux->tupla->valor3);
		aux = aux->next;
	}
	return 0;
}	

int delete(List *l, int key){
	List aux, back;

	if (*l == NULL)  // lista vacia
		return 0;

	// primer elemento de la lista
	if (key == (*l)->key){
		aux = *l;
		*l = (*l)->next;
		free(aux);
		return 0;
	}
	
	aux = *l;
	back = *l;
	while (aux!=NULL) {
		if (aux->key == key) {
			back->next = aux->next;
			free (aux);
			return 0;		// found
		}
		else {
			back = aux;
			aux = aux->next;
		}
	}

	return 0;
}	

int  destroy(List *l){
	List aux; 

	while (*l != NULL){
		aux = *l;
		*l = aux->next;
		free(aux);
	}

	return 0;
}	

