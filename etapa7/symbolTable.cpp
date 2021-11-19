#include "symbolTable.hpp"
#include <iostream>
using namespace std;

void symbolTable::insertSym(string name, ValSymTable dval )
{
	this->syms[name] = dval;
}
void symbolTable::insertSymT(string name, int tipo)
{
	this->syms[name].tipo = tipo;

}
bool symbolTable::checkForSym(string name)
{
	bool ret;
	ret = this->syms.find(name) != this->syms.end();
	return ret;
}
void symbolTable::updString(string name, int tam)
{
	this->syms[name].tam = tam;
}
void symbolTable::exportTable()
{
	for(auto itTab : this->syms)
	{

		cout << itTab.second.sect << " name: " << itTab.first << " - tipo: " << itTab.second.tipo << " - nro param: " << itTab.second.params.size() << endl;

	}
}
void symbolTable::setByName(string name,int tipo, int tam)
{
	this->syms[name].tipo = tipo;
	this->syms[name].tam = tam;
}
SymMap symbolTable::getTable()
{
	return this->syms;
}
void symbolTable::insertParams(string nomeFunc,list<struct valSymTable> listaParam)
{
	this->syms[nomeFunc].params =listaParam;
}
int symbolTable::getTypeByName(string nomeChave)
{
	return this->syms[nomeChave].tipo;
}


