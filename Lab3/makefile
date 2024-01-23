all: t1 t2 t3

t1: t1.o

t1.o: t1.c
	cc -c -lpthread t1.c

t2: t2.o

t2.o: t2.c
	cc -c -lpthread t2.c


t3: t1.o

t3.o: t3.c
	cc -c -lpthread t3.c