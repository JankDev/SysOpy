SHELL := /bin/bash

.PHONY: clean

compile: clean
		for f in `ls *.c`; do \
		gcc -o "$${f%.c}" "$$f"; \
		done
clean:
		ls -I "*.c" -I "Makefile" -I "*.h" | xargs rm -f

