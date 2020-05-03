SHELL := /bin/bash

.PHONY: clean

compile: clean
		for f in `ls *.c`; do \
		gcc -Wall -o "$${f%.c}" "$$f" -pthread; \
		done
clean:
		ls -I "*.c" -I "Makefile" -I "*.h" | xargs rm -f

