sc : main.o scanner.o keywords.o misc.o
	gcc -o sc main.o scanner.o keywords.o misc.o
main.o : main.c sc.h
	gcc -c main.c
scanner.o : scanner.c sc.h
	gcc -c scanner.c
keywords.o : keywords.c sc.h
	gcc -c keywords.c
misc.o : misc.c sc.h
	gcc -c misc.c
wc :
	wc main.c scanner.c keywords.c misc.c sc.h
clean :
	rm sc *.o
