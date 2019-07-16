sc : main.o scanner.o keywords.o misc.o parser.o encoder.o constants.o symbols.o
	gcc -o sc main.o scanner.o keywords.o misc.o parser.o encoder.o constants.o symbols.o
main.o : main.c sc.h
	gcc -c main.c
scanner.o : scanner.c sc.h
	gcc -c scanner.c
keywords.o : keywords.c sc.h
	gcc -c keywords.c
misc.o : misc.c sc.h
	gcc -c misc.c
parser.o : parser.c sc.h
	gcc -c parser.c
encoder.o : encoder.c sc.h
	gcc -c encoder.c
constants.o : constants.c sc.h
	gcc -c constants.c
symbols.o : symbols.c sc.h
	gcc -c symbols.c
wc :
	wc main.c scanner.c keywords.c misc.c paser.c sc.h
clean :
	rm sc *.o
