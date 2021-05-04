CC=clang
CFLAGS=-std=gnu11 -Wall $(shell pkg-config --cflags --libs cairo)
DEBUG_CFLAGS=-O1 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address

graphics-test: graphics-test.c
	$(CC) $(CFLAGS) graphics-test.c -o graphics-test

debug: graphics-test.c
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) graphics-test.c -o graphics-test
