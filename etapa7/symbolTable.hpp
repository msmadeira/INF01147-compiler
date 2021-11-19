#include <list>
#include <string>
#include "tree.h"
#include <unordered_map>
using namespace std;
struct valSymTable
{
	int line;
	int sect;
	int tipo;
	int tam;
	list<struct valSymTable> params;
	lexic_val_type lexVal;
};

typedef struct valSymTable ValSymTable; 
typedef unordered_map<string,ValSymTable> SymMap;

class symbolTable{
	public:
		void insertSym(string name, ValSymTable dval );
		SymMap getTable();
		void insertSymT(string name, int tipo);
		bool checkForSym(string name);
		void exportTable();
		void setByName(string name,int tipo,int tam);
		void insertParams(string nomeFunc,list<struct valSymTable> listaParam);
		int getTypeByName(string nomeChave);
		void updString(string name, int tam);
	private:
		SymMap syms;
};