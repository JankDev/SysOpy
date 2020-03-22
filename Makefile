compile:
		gcc -c tests.c time.h time.c
link: compile
		gcc -o tests tests.o time.o
run: link
		./tests dummy2
clean:
		rm *.o
.PHONY: clean
