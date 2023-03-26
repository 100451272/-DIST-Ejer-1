
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include "claves.h"

int main(int argc, char **argv) {
    if (argc < 2){
        perror("Faltan argumentos. Uso: ./cliente [comando] [argumentos]");
        return -1;
    }
    char* command = argv[1];
    int res;
    if (strcmp(command, "init") == 0){
        init();
        
    }


    else if (strcmp(command, "set_value") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        char *value1 = argv[3];
        int value2 = strtol(argv[4], &ptr, 10);
        double value3 = strtod(argv[5], &ptr);
        res = set_value(clave, value1, value2, value3);
    }

    else if (strcmp(command, "exists") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        printf("Exists1\n");
        res = exist(clave);
        printf("Exists10\n");
        if (res == 1) {
            printf("La clave existe\n");
        } else if (res == 0) {
            printf("La clave no existe\n");
        } else {
            perror("Error en las comunicaciones");
            return -1;
        }
    }

    else if (strcmp(command, "modify_value") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        char *value1 = argv[3];
        int value2 = strtol(argv[4], &ptr, 10);
        double value3 = strtod(argv[5], &ptr);
        res = modify_value(clave, value1, value2, value3);
    }


    else if (strcmp(command, "copy_key") == 0){
        char *ptr;
        int key1 = strtol(argv[2], &ptr, 10);
        int key2 = strtol(argv[3], &ptr, 10);
        res = copy_key(key1, key2);
        if (res == 0) {
            printf("Clave creada y valores copiados correctamente\n");
        } else if (res == -1) {
            printf("La clave %d no existe, no se puede crear la nueva clave %d\n", key1, key2);
        } else {
            perror("Error en las comunicaciones");
            return -1;
        }
    }


    else if (strcmp(command, "copy_key") == 0){
        char *ptr;
        int key1 = strtol(argv[2], &ptr, 10);
        int key2 = strtol(argv[3], &ptr, 10);

        // Verificar si key1 existe
        if (exist(key1) == 0) {
            printf("La clave %d no existe\n", key1);
            res = -1;
        } else {
            // Si key2 no existe, crearla y copiar los valores de key1
            if (exist(key2) == 0) {
                if (copy_key(key1, key2) == 0) {
                    printf("Clave %d creada y copiada correctamente en clave %d\n", key1, key2);
                    res = 0;
                } else {
                    printf("Error al crear y copiar clave %d en clave %d\n", key1, key2);
                    res = -1;
                }
            } else {
                // Si key2 ya existe, modificar sus valores con los de key1
                if (modify_value(key1, key2) == 0) {
                    printf("Valores de clave %d copiados correctamente en clave %d\n", key1, key2);
                    res = 0;
                } else {
                    printf("Error al modificar valores de clave %d en clave %d\n", key1, key2);
                    res = -1;
                }
            }
        }
    }


        /*else if (strcmp(command, "copy_key") == 0){
            char *ptr;
            int key1 = strtol(argv[2], &ptr, 10);
            int key2 = strtol(argv[3], &ptr, 10);
            res = copy_key(key1, key2);
            if(res == -1){
                printf("Error al copiar la clave %d en la clave %d\n", key1, key2);
            }
            else{
                printf("La clave %d ha sido copiada correctamente en la clave %d\n", key1, key2);
            }
        }*/





    else if (strcmp(command, "get_value") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        char value1[255];
        int value2;
        double value3;
        res = get_value(clave, value1, &value2, &value3);
        if (res == 0){
            printf("Valor 1: %s\nValor 2: %d\nValor 3: %f\n", value1, value2, value3);
        } else {
            printf("No se ha encontrado un elemento con clave %d\n", clave);
        }
    }

    else if (strcmp(command, "delete_key") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        res = delete_key(clave);
        if (res == 0) {
            printf("Elemento con clave %d eliminado.\n", clave);
        } else {
            printf("No se ha encontrado un elemento con clave %d.\n", clave);
        }
    }






    return res;
}
