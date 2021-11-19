all:	
	bison -d parser.y
	flex scanner.l
	g++ -std=c++0x -c lex.yy.c parser.tab.c main.cpp 
	g++ -std=c++0x -o etapa4 tree.c util.c stackB.cpp symbolTable.cpp lex.yy.o main.o parser.tab.o -lfl
clean:
	rm main.o lex.yy.o lex.yy.c etapa4 parser.tab.o parser.tab.c parser.tab.h 
