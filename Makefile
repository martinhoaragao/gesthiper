all: clients sales gesthiper

CFLAGs= -ansi -Wall -pedantic -O2

gesthiper: gesthiper.c
	gcc gesthiper.c clients.o $(CFLAGs) -o gesthiper

clients: clients.c clients.h
	gcc $(CFLAGS) -c clients.c 

clientstest: tests/clientstest.c clients.c clients.h
	make clients && gcc tests/clientstest.c clients.o $(CFLAGS) -o tests/clientstest

accounting: accounting.c accounting.h
	gcc accounting.c -c $(CFLAGS)

accountingtest: tests/accountingtest.c accounting.c accounting.h
	make accounting && gcc tests/accountingtest.c accounting.o $(CFLAGS) -o tests/accountingtest

sales: sales.c sales.h
	gcc sales.c -c $(CFLAGS)

clear:
	rm clients.o tests/clientstest accounting.o tests/accountingtest
