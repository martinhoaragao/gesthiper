all: clients sales gesthiper

CFLAGs=-ansi -Wall -pedantic -O2

gesthiper: gesthiper.c
	gcc gesthiper.c clients.o $(CFLAGs) -o gesthiper

clients: clients.c clients.h
	gcc clients.c -c $(CFLAGS)

clientstest: tests/clientstest.c clients.c clients.h
	make clients && gcc tests/clientstest.c clients.o $(CFLAGS) -o tests/clientstest

sales: sales.c sales.h
	gcc sales.c -c $(CFLAGS)
