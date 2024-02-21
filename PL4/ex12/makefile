main: main.o
	gcc -Wall -g main.o -o main -pthread -lrt
	gcc -Wall -g client.o -o client -pthread -lrt

main.o: main.c
	gcc -Wall -g -c main.c
	gcc -Wall -g -c client.c

run: main
	./main

clean:
	rm *.o main 
	rm *.o client