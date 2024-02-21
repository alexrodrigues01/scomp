main: main.o
	gcc -Wall -g writer.o -o writer -pthread -lrt
	gcc -Wall -g reader.o -o reader -pthread -lrt
	gcc -Wall -g main.o -o main -pthread -lrt

main.o: writer.c reader.c main.c
	gcc -Wall -g -c writer.c
	gcc -Wall -g -c reader.c
	gcc -Wall -g -c main.c

run: main
	./main

clean:
	rm *.o writer reader main