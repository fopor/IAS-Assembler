mainout: main.o mnemonicos.o  line_solver.o diretivas.o
	gcc main.o mnemonicos.o line_solver.o diretivas.o -o ias-as

main:  main.c
	gcc  -c main.c -o main.o

linesolver: line_solver.c
	gcc -c line_solver.c -o line_solver.o

mnemonico: mnemonicos.c
	gcc  -c mnemonicos.c -o mnemonicos.o
	
diretivas: diretivas.c
	gcc -c diretivas.c -o diretivas.o

force: mnemonicos.c line_solver.c main.c diretivas.c main.o mnemonicos.o  line_solver.o
	gcc  -c mnemonicos.c -o mnemonicos.o
	gcc -c line_solver.c -o line_solver.o
	gcc  -c main.c -o main.o
	gcc  -g -c diretivas.c -o diretivas.o
	gcc main.o mnemonicos.o line_solver.o diretivas.o -o ias-as
	
debug: mnemonicos.c line_solver.c main.c diretivas.c main.o mnemonicos.o  line_solver.o diretivas.o
	clear
	gcc  -g -c mnemonicos.c -o mnemonicos.o
	gcc -g -c line_solver.c -o line_solver.o
	gcc  -g -c main.c -o main.o
	gcc  -g -c diretivas.c -o diretivas.o
	gcc -g main.o mnemonicos.o line_solver.o diretivas.o -o ias-as