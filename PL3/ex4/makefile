reader: reader.o
	gcc -Wall -g reader.o -o reader -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c

writer: writer.o
	gcc -Wall -g writer.o -o writer -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c

run: writer reader
	./writer
	./reader

clean:
	rm *.o writer reader