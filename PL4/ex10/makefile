main: main.o
	gcc -Wall -g main.o -o main -pthread -lrt
	gcc -Wall -g insert.o -o insert -pthread -lrt
	gcc -Wall -g consult.o -o consult -pthread -lrt
	gcc -Wall -g consult_All.o -o consult_All -pthread -lrt


main.o: main.c
	gcc -Wall -g -c main.c
	gcc -Wall -g -c insert.c
	gcc -Wall -g -c consult.c
	gcc -Wall -g -c consult_All.c

run: main
	./main

clean:
	rm *.o main 