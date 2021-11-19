#include <list>
#include <string>
#include "symbolTable.hpp"
#include "errors.h"

typedef list<symbolTable> StackMaps;
struct inicPend
{
	lexic_val_type* variable;
	lexic_val_type* value;	
};
int getSizeT(int tipo);
bool testVi(int tipoOrig,int tipoDst);
typedef struct inicPend PendingInic;
string getNamelv(lexic_val_type* valLex);
class StackM
{
	public:
		StackM();
		void insertNewSM();
		symbolTable delSM();
		void addNormal(int line, int sect, lexic_val_type *valLex,int tipo);
		void addNewS(int line, int sect, lexic_val_type *valLex,int tipo,int tam);
		void updateTS(int tipo);
		void addParam(string name,string nome_par);
		void addSymA(string name, ValSymTable newSymbol);
		bool checkSym(string name);
		bool checkSymA(string name);
		ValSymTable getSym(string name);
		void exportaTabelas();
		void tError(int tipoErro, int line, string name,string aux);
		void insertFun(int line, lexic_val_type *valLex );
		void updateFun(lexic_val_type *valLex,int tipo );
		void pushParam(lexic_val_type *valLex);
		int typeinf(a_node* nodoEsq,a_node* nodoDir);
		int verifyFunc(lexic_val_type *valLex, a_node * node,int line);
		int verifyArr(lexic_val_type *valLex, a_node * vetIndex);
		int verifyV(lexic_val_type *valLex);
		int infTT(a_node* nodoEsq,a_node* mNode,a_node* nodoDir);
		ValSymTable retSymb(string name);
		bool compParams(list<ValSymTable> listParam,a_node* node, int line,string nomeFunc );
		void verifyAt(a_node* nodoDst,a_node* nodoOrig);
		int getTypeLex(lexic_val_type *valLex);
		ValSymTable getSymbLex(lexic_val_type *valLex);
		int avalIn(lexic_val_type *valLex);
		int avalOut(lexic_val_type *valLex);
		void avalShift(lexic_val_type *valLex);
		void verifyRet(int tipoRet,int line);
		int getTypeF();
		void addIni(lexic_val_type *valorVariable,lexic_val_type *valorValue);
		void fazInic();
	private:
		StackMaps circ;
		list<struct valSymTable> pendingP;
		list<PendingInic> pendingI;
};