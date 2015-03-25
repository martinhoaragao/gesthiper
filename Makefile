CFLAGS=-Wall -ansi -pedantic -O2

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	CFLAGS += -g -fno-stack-protector -D_FORTIFY_SOURCE=0
endif

gesthiper: gesthiper.c
	gcc gesthiper.c clients.o $(CFLAGS) -o gesthiper

clients: clients.c clients.h
	gcc $(CFLAGS) -c clients.c

clientstest: tests/clientstest.c clients.c clients.h
	make clients && gcc tests/clientstest.c clients.o $(CFLAGS) -o tests/clientstest
	
products: products.c products.h
	gcc $(CFLAGS) -c products.c

productstest: tests/productstest.c products.c products.h
	make products && gcc tests/productstest.c products.o $(CFLAGS) -o tests/productstest

accounting: accounting.c accounting.h
	gcc accounting.c -c $(CFLAGS)

accountingtest: tests/accountingtest.c accounting.c accounting.h
	make accounting && gcc tests/accountingtest.c accounting.o $(CFLAGS) -o tests/accountingtest

sales: sales.c sales.h
	gcc sales.c -c $(CFLAGS)

clear:
	rm clients.o tests/clientstest accounting.o tests/accountingtest
