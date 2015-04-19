CFLAGS=-Wall -ansi -pedantic -O2
CFLAGSVALGRIND=-Wall -ansi -pedantic

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	CFLAGS += -g -fno-stack-protector -D_FORTIFY_SOURCE=0
	CFLAGSVALGRIND += -g -fno-stack-protector -D_FORTIFY_SOURCE=0
endif

gesthiper: gesthiper.c
	make clients
	make products
	make accounting
	make sales
	gcc gesthiper.c clients.o products.o accounting.o sales.o $(CFLAGS) -o gesthiper -lm

clients_avl: clients_avl.c clients_avl.h
	gcc $(CFLAGS) -c clients_avl.c

clients_avl_test: clients_avl.o clients_avl.h tests/clients_avl_test.c
	make clients_avl
	gcc tests/clients_avl_test.c clients_avl.o $(CFLAGS) -o tests/clients_avl

clients: clients.c clients.h
	gcc $(CFLAGS) -c clients.c

clientstest: tests/clientstest.c clients.c clients.h
	make clients
	gcc tests/clientstest.c clients.o $(CFLAGS) -o tests/clientstest

products: products.c products.h
	gcc $(CFLAGS) -c products.c

productstest: tests/productstest.c products.c products.h
	make products
	gcc tests/productstest.c products.o $(CFLAGS) -o tests/productstest

accounting: accounting.c accounting.h
	gcc accounting.c -c $(CFLAGS)

accountingtest: tests/accountingtest.c accounting.c accounting.h
	make accounting
	make clients
	make products
	gcc -g tests/accountingtest.c accounting.o  products.o clients.o $(CFLAGSVALGRIND ) -o tests/accountingtest

sales: sales.c sales.h
	gcc sales.c -c $(CFLAGS)

salestest: sales.o sales.h tests/salestest.c
	make sales
	make products
	make clients
	gcc tests/salestest.c sales.o clients.o products.o $(CFLAGS) -o tests/salestest

report: report/report.tex
	pdflatex report/report.tex

clean:
	rm -f *.o
	rm -f tests/clientstest
	rm -f tests/accountingtest
	rm -f tests/productstest
	rm gesthiper
