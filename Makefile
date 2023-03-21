CC = gcc

all: cliente servidor

cliente: cliente.o libclaves.so
	$(CC) cliente.o -Wl,-rpath=. -L. -lclaves -o cliente

cliente.o: cliente.c
	$(CC) -Wall -o cliente.o -c cliente.c

claves.o: claves.c
	$(CC) -Wall -o claves.o -fPIC -c claves.c

libclaves.so: claves.o
	$(CC) -shared claves.o -o libclaves.so

servidor: servidor.o lista.o
	$(CC) -Wall servidor.o lista.o -o servidor

servidor.o: servidor.c
	$(CC) -Wall -c servidor.c -o servidor.o

lista.o: lista.c
	$(CC) -Wall -c lista.c -o lista.o

clean:
	rm -f *~ *.o libclaves.so cliente servidor