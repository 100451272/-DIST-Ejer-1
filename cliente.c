
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

    else if (strcmp(command, "get_value") == 0){
        char *ptr;
        int clave = strtol(argv[2], &ptr, 10);
        char *value1;
        int *value2;
        value2 = 0;
        double *value3;
        value3 = 0;
        res = get_value(clave, &value1, value2, value3);
    }

    return res;
}
