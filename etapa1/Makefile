all:
	flex scanner.l
	gcc -c lex.yy.c -lfl
	gcc -c main.c
	gcc lex.yy.o main.o -o etapa1 -lfl
clean:
	rm etapa1 lex.yy.c lex.yy.o main.o etapa1.tgz