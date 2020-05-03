SHELL := /bin/bash

.PHONY: clean

compile: clean
		for f in `ls *.c`; do \
		gcc -o "$${f%.c}" "$$f" -lrt; \
		done
clean:
		ls -I "*.c" -I "Makefile" -I "*.h" -I "*.txt" | xargs rm -f

