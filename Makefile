build:
	gcc -std=c9x movies.c -Wall -g -o movies
run:
	./movies
valgrind:
	valgrind ./movies -c1 tests/task1/in/test0.in tests/task1/out/test0.out
clean:
	rm movies *~

