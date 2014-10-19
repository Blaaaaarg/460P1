#Makefile created by maker v1.4	github.com/Blaaaaarg/MakefileScript
walkerP1.out : walkerP1.o SetLimits.o
	gcc -o P1.out walkerP1.o SetLimits.o
walkerP1.o : Project1.c
	gcc -c Project1.c -o walkerP1.o
SetLimits.o : SetLimits.c SetLimits.h
	gcc -c SetLimits.c -o SetLimits.o
lex.o : lex.c lex.h
	gcc -c lex.c -o lex.o
clean :
	rm -f *.o *.x *.out
run :
	./walkerP1.out
