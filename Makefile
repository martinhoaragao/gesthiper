CFLAGS=-Wall -ansi -pedantic -O2

all:
	make clean
	make clients
	make products
	make accounting
	make salesc
	make salesp
	make gesthiper

gesthiper: src/gesthiper.c clients.o products.o accounting.o salesc.o salesp.o
	gcc src/gesthiper.c clients.o products.o accounting.o salesc.o salesp.o $(CFLAGS) -o gesthiper -lm

clients: src/clients.c src/clients.h
	gcc src/clients.c -c $(CFLAGS)

clientstest: tests/clientstest.c clients.o src/clients.h
	gcc tests/clientstest.c clients.o $(CFLAGS) -o tests/clientstest

products: src/products.c src/products.h
	gcc src/products.c -c $(CFLAGS)

productstest: tests/productstest.c products.o src/products.h
	gcc tests/productstest.c products.o $(CFLAGS) -o tests/productstest

accounting: src/accounting.c src/accounting.h
	gcc src/accounting.c -c $(CFLAGS)

accountingtest: tests/accountingtest.c accounting.o src/accounting.h clients.o products.o
	gcc -g tests/accountingtest.c accounting.o products.o clients.o $(CFLAGS) -o tests/accountingtest

salesc: src/salesc.c src/salesc.h
	gcc src/salesc.c -c $(CFLAGS)

salesctest: tests/salesctest.c salesc.o clients.o products.o src/salesc.h src/clients.h src/products.h
	gcc tests/salesctest.c salesc.o clients.o products.o -o tests/salesctest $(CFLAGS)

salesp: src/salesp.c src/salesp.h
	gcc src/salesp.c -c $(CFLAGS)

report: report/report.tex
	pdflatex report/report.tex

clean:
	-rm -f *.o tests/*test gesthiper results/*
