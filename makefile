CC = gcc
CFLAGS = -std=c89 -pedantic


all:  master Processo_Nodo Processo_Utente 
clean: 
	rm -f *.o
	rm master
	rm Processo_Nodo
	rm Processo_Utente

