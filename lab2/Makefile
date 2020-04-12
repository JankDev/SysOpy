compile:
		gcc -c tests.c time.h time.c
link: compile
		gcc -o tests tests.o time.o
run: link
		./tests dummy2
clean:
		ls -I "*.c" -I "Makefile" -I "*.h" | xargs rm -f
.PHONY: clean
