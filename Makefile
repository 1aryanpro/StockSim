CC=clang
CFLAGS=-std=gnu11 -Wall
CFLAGS+=$(shell curl-config --cflags) $(shell curl-config --libs)
CFLAGS+=-I/usr/local/include -I/opt/homebrew/include -lcjson
CFLAGS+=$(shell pkg-config --cflags --libs cairo SDL2)

DEBUG_CFLAGS=-O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address

stocks: download.c download.h stocks.c
	$(CC) $(CFLAGS) download.c stocks.c -o build/stocks

download: download.c
	$(CC) $(CFLAGS) download.c -o build/download

debug:  download.c download.h stocks.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) download.c stocks.c -o build/stockDebug

graphics: download.c download.h stocks.c stocks.h graphs.c
	$(CC) $(CFLAGS) download.c stocks.c graphs.c -o build/graphics

debug-graphics: download.c download.h stocks.c stocks.h graphs.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) download.c stocks.c graphs.c -o build/graphicsDebug
