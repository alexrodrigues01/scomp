main: main.o  
	gcc -Wall -g main.o -o main -pthread -lrt
	gcc -Wall -g normal.o -o normal -pthread -lrt
	gcc -Wall -g vip.o -o vip -pthread -lrt
	gcc -Wall -g especial.o -o especial -pthread -lrt

main.o: main.c normal.c vip.c especial.c
	gcc -Wall -g -c main.c
	gcc -Wall -g -c normal.c
	gcc -Wall -g -c vip.c
	gcc -Wall -g -c especial.c
run: main
	./main

clean:
	rm *.o main normal vip