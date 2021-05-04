CC=clang
CFLAGS=-std=gnu11 -Wall $(shell curl-config --cflags) -I/usr/local/include -I/opt/homebrew/include $(shell curl-config --libs) -lcjson
DEBUG_CFLAGS=-O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address

download: download.c
	$(CC) $(CFLAGS) download.c -o download

debug: download.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) download.c -o download

stocks: download.c download.h stocks.c
	$(CC) $(CFLAGS) download.c stocks.c -o stocks