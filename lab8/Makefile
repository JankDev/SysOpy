SHELL := /bin/bash

.PHONY: clean

run_server: compile
		./server
compile: clean
		gcc -o client client.c && gcc -o server server.c
clean:
		ls -I "*.c" -I "Makefile" -I "*.h" | xargs rm -f

