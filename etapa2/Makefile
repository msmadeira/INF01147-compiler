all:
	bison -d parser.y
	flex scanner.l
	gcc -c lex.yy.c -lfl
	gcc -c main.c parser.tab.c
	gcc lex.yy.o main.o parser.tab.o -o etapa2 -lfl
clean:
	rm etapa2 parser.tab.h parser.tab.c parser.tab.o lex.yy.c lex.yy.o main.o