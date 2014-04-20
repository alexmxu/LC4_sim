all: trace
ObjectFiles: LC4.h
	clang -c ObjectFiles.c
LC4: LC4.h
	clang -c LC4.c
trace: trace.c ObjectFiles.o LC4.o
	clang -o trace trace.c ObjectFiles.o LC4.o
clean:
	rm –rf *.o