detect: detect.c graph.o list.o util.o
	gcc -Wall -o detect detect.c graph.o list.o util.o -g

list.o: list.c list.h
	gcc -Wall -c list.c list.h -g

graph.o: graph.c graph.h
	gcc -Wall -c graph.c graph.h -g

util.o: util.c util.h
	gcc -Wall -c util.c util.h -g

clean:
	rm -rfv detect *.o *gch *.dSYM


