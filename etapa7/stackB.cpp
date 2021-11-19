#include "stackB.hpp"
#include <iostream>
using namespace std;

StackM::StackM()
{
	this->insertNewSM();
}

void StackM::insertNewSM()
{
	symbolTable contexto;
	this->circ.push_back(contexto);
}

symbolTable StackM::delSM()
{
	symbolTable mapaRet = this->circ.back();

	this->circ.pop_back();

	return mapaRet;
}
string getNamelv(lexic_val_type* valLex)
{
	string kname;
	if(valLex->type == TIPO_LIT)
	{
		switch (valLex->value_type)
		{
		case LIT_TIPO_INT:
			kname = string(to_string(valLex->tk_value.vInt));
			break;
		case LIT_TIPO_BOOL:
			kname = string(to_string(valLex->tk_value.vBool));
			break;
		case LIT_TIPO_CHAR:
			kname = string(valLex->tk_value.vChar);
			break;
		case LIT_TIPO_FLOAT:
			kname = string(to_string(valLex->tk_value.vFloat));
			break;
		case LIT_TIPO_STRING:
			kname = string(valLex->tk_value.vStr);
			break;
		default:
			kname = string(valLex->tk_value.vStr);
			break;
		}
	}
	else
	{
		kname = string(valLex->tk_value.vStr);
	}
	return kname;
}

int getSizeT(int tipo)
{
	int ret;
	switch (tipo)
	{
	case ID_INT:
		ret=4;
		break;
	case ID_FLOAT:
		ret=8;
		break;
	case ID_BOOL:
		ret=1;
		break;
	case ID_CHAR:
		ret=1;
		break;
	case ID_STRING:
		ret =0;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

void StackM::addNormal(int line, int sect, lexic_val_type *valLex, int tipo)
{
	ValSymTable newSymbol;
	list<ValSymTable> params;
	string kname,nomeOrg;
	newSymbol.line = line;
	newSymbol.sect = sect;
	newSymbol.tipo = tipo;
	newSymbol.tam = 1;
	if(sect == NAT_LIT)
	{
		newSymbol.tam = getSizeT(newSymbol.tipo);
		switch(newSymbol.tipo)
		{			
			case LIT_TIPO_INT:
				kname = string(to_string(valLex->tk_value.vInt));
				break;
			case LIT_TIPO_BOOL:
				kname = string(to_string(valLex->tk_value.vBool));
				break;
			case LIT_TIPO_CHAR:
				kname = string(valLex->tk_value.vChar);
				break;
			case LIT_TIPO_FLOAT:
				kname = string(to_string(valLex->tk_value.vFloat));
				break;
			case LIT_TIPO_STRING:
				kname = string(valLex->tk_value.vStr);
				newSymbol.tam = kname.size(); 
				break;
			case INDEF:
				kname = string(valLex->tk_value.vStr);
				break;
			default:
				kname = string(valLex->tk_value.vStr);
				break;

		}

		nomeOrg = kname; 
		kname.append("LIT");
	}	
	else
	{
		kname = string(valLex->tk_value.vStr);
		nomeOrg = kname; 
		if(this->checkSymA(kname))
			this->tError(ERR_DECLARED,line,nomeOrg,kname);
	}

	newSymbol.lexVal = *valLex;
	newSymbol.params = params;

	this->addSymA(kname, newSymbol);
}
void StackM::addNewS(int line, int sect, lexic_val_type *valLex,int tipo,int tam)
{
	ValSymTable newSymbol;
	list<ValSymTable> params;
	string kname,nomeOrg;
	newSymbol.line = line;
	newSymbol.sect = sect;
	newSymbol.tipo = tipo;
	newSymbol.tam = tam;
	switch(newSymbol.tipo)
	{
		case LIT_TIPO_INT:
			kname = string(to_string(valLex->tk_value.vInt));
			break;
		case LIT_TIPO_BOOL:
			kname = string(to_string(valLex->tk_value.vBool));
			break;
		case LIT_TIPO_CHAR:
			kname = string(valLex->tk_value.vChar);
			break;
		case LIT_TIPO_FLOAT:
			kname = string(to_string(valLex->tk_value.vFloat));
			break;
		case LIT_TIPO_STRING:
			kname = string(valLex->tk_value.vStr);
			break;
		case INDEF:
			kname = string(valLex->tk_value.vStr);
			break;
		default:
			kname = string(valLex->tk_value.vStr);
			break;

	}
	nomeOrg = kname; 
	newSymbol.lexVal = *valLex;
	newSymbol.params = params;
	if(this->checkSymA(kname))
	{
		this->tError(ERR_DECLARED,line,nomeOrg,kname);
	}
	this->addSymA(kname,newSymbol);
}

void StackM::addIni(lexic_val_type *valorVariable,lexic_val_type *valorValue)
{
	int tipoValue;
	PendingInic ini;
	if(valorValue->type != TIPO_LIT)
	{
		tipoValue = this->verifyV(valorValue);
	}
	ini.variable =valorVariable;
	ini.value =valorValue;
	this->pendingI.push_back(ini);
}

void StackM::fazInic()
{
	ValSymTable var,value;
	int line;
	string name;
	for(auto ini : this->pendingI)
	{
		var = this->getSymbLex(ini.variable);
		value = this->getSymbLex(ini.value);

		line = ini.value->lineno;
		name = string(ini.variable->tk_value.vStr);
		if(value.tipo == ID_CHAR && var.tipo != ID_CHAR )
		{
			string msg = name;
			name = string(ini.value->tk_value.vStr);
			this->tError(ERR_CHAR_TO_X,line,name,msg);
		}
		if(value.tipo == ID_STRING && var.tipo != ID_STRING )
		{
			string msg;
			msg = name;
			name = string(ini.value->tk_value.vStr);
			this->tError(ERR_STRING_TO_X,line,name,msg);
		}

		if(value.tipo!=var.tipo && !(testVi(value.tipo,var.tipo)))
		{
			string msg;
			msg= string(ini.value->tk_value.vStr);
			this->tError(ERR_WRONG_TYPE,line,name,msg);
		}
		if(var.tipo == ID_STRING && value.tipo == ID_STRING )
		{
			this->circ.back().updString(name,value.tam);
		}
		
	}
	this->pendingI.clear();
}


void StackM::insertFun(int line, lexic_val_type *valLex )
{
	ValSymTable newSymbol;
	list<ValSymTable> params;
	string name = string(valLex->tk_value.vStr);
	newSymbol.sect = NAT_FUN;
	newSymbol.tipo = INDEF;
	newSymbol.params = params;
	newSymbol.lexVal = *valLex;
	newSymbol.tam = 1;
	if(this->checkSymA(name))
		this->tError(ERR_DECLARED,line,name,name);
	
	this->circ.front().insertSym(name,newSymbol);

}

void StackM::pushParam(lexic_val_type *valLex)
{
	string name = string(valLex->tk_value.vStr);
	ValSymTable dval = this->getSym(name);
	this->pendingP.push_back(dval);
}

int StackM::typeinf(a_node* nodoEsq,a_node* nodoDir)
{
	int tipoEsq = nodoEsq->typeSemVal;
	int tipoDir = nodoDir->typeSemVal;

	if((tipoDir != tipoEsq) &&(tipoEsq == ID_STRING || tipoDir == ID_STRING))
	{
		tError(ERR_STRING_TO_X,nodoEsq->lexValue->lineno,"","");
	}
	if((tipoDir != tipoEsq) &&(tipoEsq == ID_CHAR || tipoDir == ID_CHAR))
	{
		tError(ERR_STRING_TO_X,nodoEsq->lexValue->lineno,"","");
	}
	if(tipoEsq == tipoDir)
		return tipoDir;
	if(tipoDir == INDEF ||tipoEsq == INDEF || tipoDir == ID_STRING || tipoEsq == ID_STRING || tipoDir == ID_CHAR || tipoEsq ==ID_CHAR )
		return INDEF;
	if((tipoEsq == ID_INT && tipoDir == ID_INT)||(tipoEsq == ID_INT && tipoDir == ID_BOOL)|| (tipoEsq == ID_BOOL && tipoDir == ID_INT))
		return ID_INT;
	if(tipoEsq == ID_FLOAT || tipoDir == ID_FLOAT)
		return ID_FLOAT;
	
	return ID_BOOL;
}

int StackM::infTT(a_node* nodoEsq,a_node* mNode,a_node* nodoDir)
{
	int tipo_temp;
	a_node* tmpNode;
	tmpNode = (a_node*)malloc(sizeof(a_node));

	tipo_temp = this->typeinf(nodoEsq,mNode);
	tmpNode->typeSemVal= tipo_temp;
	tipo_temp = this->typeinf(tmpNode,nodoDir);

	free(tmpNode);

	return tipo_temp;
}
bool testVi(int tipoOrig,int tipoDst)
{
	bool ret = false;
	if(tipoOrig == ID_BOOL &&(tipoDst == ID_INT || tipoDst == ID_FLOAT ))
		ret = true;
	if(tipoOrig == ID_INT &&(tipoDst == ID_BOOL || tipoDst == ID_FLOAT ))
		ret = true;
	if(tipoOrig == ID_FLOAT &&(tipoDst == ID_INT || tipoDst == ID_BOOL ))
		ret = true;
	return ret;
}

bool StackM::compParams(list<ValSymTable> listParam,a_node* node,int line, string nomeFunc )
{
	int inputSize = 0;
	list<int> inputTipeList;
	while(node !=NULL)
	{
		inputSize++;
		inputTipeList.push_back(node->typeSemVal);
		node = node->child;
		if(node!=NULL)
		{
			while(node->nextBro != NULL)
				node = node->nextBro;
			if(node->is_arg == false)
				node = NULL;
		}
	}
	if(inputSize > listParam.size())
	{
		string msg = "Expected "+ to_string(listParam.size()) +" received " + to_string(inputSize)+".";
		this->tError(ERR_EXCESS_ARGS,line,nomeFunc,msg);
	}
	if(inputSize < listParam.size())
	{
		string msg = "Expected "+ to_string(listParam.size()) +" received " + to_string(inputSize)+".";
		this->tError(ERR_MISSING_ARGS,line,nomeFunc,msg);
	}

	int iteration = 0;
	list<int>::iterator it1 = inputTipeList.begin();
	list<ValSymTable>::iterator it2 =listParam.begin();
	for(; it1 != inputTipeList.end() && it2 != listParam.end(); ++it1, ++it2)
	{

		iteration++;
		if((*it1)==ID_STRING)
		{
			string msg;
			msg= "received parameter of illegal type STRING.";
			this->tError(ERR_FUNCTION_STRING,line,nomeFunc,msg);
		}
		
		if((*it1)!=(*it2).tipo && !(testVi((*it1),(*it2).tipo)))
		{
			string msg;
			msg= "received argument number " + to_string(iteration) +" with incopatible type.";
			this->tError(ERR_WRONG_TYPE_ARGS,line,nomeFunc,msg);
		}

	}
	return true;

}

int StackM::verifyFunc(lexic_val_type *valLex, a_node* node,int line)
{
	string nomeFunc = string(valLex->tk_value.vStr);
	bool existe = this->checkSym(nomeFunc);
	ValSymTable dadoFunc;
	if(!existe)
	{
		tError(ERR_UNDECLARED,valLex->lineno,nomeFunc,nomeFunc);
	}
	dadoFunc = this->retSymb(nomeFunc);

	if(dadoFunc.sect == NAT_VAR || dadoFunc.sect == NAT_VET)
	{

		if(dadoFunc.sect == NAT_VAR)
			this->tError(ERR_VARIABLE,valLex->lineno,nomeFunc,"Function");
		else
			this->tError(ERR_VECTOR,valLex->lineno,nomeFunc,"Function");

	}
	this->compParams(dadoFunc.params,node,line,nomeFunc);

	return dadoFunc.tipo;
}

int StackM::verifyArr(lexic_val_type *valLex, a_node* vetIndex)
{
	string nomeVet = string(valLex->tk_value.vStr);
	bool existe = this->checkSym(nomeVet);
	ValSymTable dadoVet;
	if(!existe)
	{
		tError(ERR_UNDECLARED,valLex->lineno,nomeVet,nomeVet);
	}
	dadoVet = this->retSymb(nomeVet);

	if(dadoVet.sect == NAT_FUN || dadoVet.sect == NAT_VAR)
	{

		if(dadoVet.sect == NAT_VAR)
			this->tError(ERR_VARIABLE,valLex->lineno,nomeVet,"Vector");
		else
			this->tError(ERR_FUNCTION,valLex->lineno,nomeVet,"Vector");

	}
	if(vetIndex->typeSemVal!=ID_INT && !(testVi(ID_INT,vetIndex->typeSemVal)))
		this->tError(ERR_WRONG_TYPE,valLex->lineno,nomeVet,"Array");
		
	return dadoVet.tipo;
}

int StackM::verifyV(lexic_val_type *valLex)
{
	string nomeVar = string(valLex->tk_value.vStr);
	bool existe = this->checkSym(nomeVar);
	ValSymTable dadoVar;
	if(!existe)
	{
		tError(ERR_UNDECLARED,valLex->lineno,nomeVar,nomeVar);
	}
	dadoVar = this->retSymb(nomeVar);

	if(dadoVar.sect == NAT_FUN || dadoVar.sect == NAT_VET)
	{

		if(dadoVar.sect == NAT_FUN)
			this->tError(ERR_FUNCTION,valLex->lineno,nomeVar,"Variable");
		else
			this->tError(ERR_VECTOR,valLex->lineno,nomeVar,"Variable");

	}

	return dadoVar.tipo;
}

void StackM::updateFun(lexic_val_type *valLex,int tipo)
{
	string kname = string(valLex->tk_value.vStr);
	if(tipo == ID_STRING)
	{
		string msg;
		msg= "received illegal return type STRING.";
		this->tError(ERR_FUNCTION_STRING,valLex->lineno,kname,msg);
	}
	for(auto parametro:this->pendingP)
	{
		if(parametro.tipo == ID_STRING)
		{
			string msg;
			msg= "received a parameter with illegal type STRING.";
			this->tError(ERR_FUNCTION_STRING,valLex->lineno,kname,msg);
		}
	}
	this->circ.front().insertParams(kname,this->pendingP);
	this->circ.front().setByName(kname,tipo,getSizeT(tipo));
	this->pendingP.clear();
}


bool StackM::checkSymA(string name)
{
	return this->circ.back().checkForSym(name);
}

void StackM::exportaTabelas()
{
	for(auto contexto : this->circ)
	{
		cout << "nova tabela : "<< endl;
		contexto.exportTable();
	}
		
}

int StackM::getTypeLex(lexic_val_type *valLex)
{
	string kname ;
	if(valLex->type == TIPO_LIT)
	{
		switch (valLex->value_type)
		{
		case LIT_TIPO_INT:
			kname = string(to_string(valLex->tk_value.vInt));
			break;
		case LIT_TIPO_BOOL:
			kname = string(to_string(valLex->tk_value.vBool));
			break;
		case LIT_TIPO_CHAR:
			kname = string(valLex->tk_value.vChar);
			break;
		case LIT_TIPO_FLOAT:
			kname = string(to_string(valLex->tk_value.vFloat));
			break;
		case LIT_TIPO_STRING:
			kname = string(valLex->tk_value.vStr);
			break;
		default:
			kname = string(valLex->tk_value.vStr);
			break;
		}
		kname.append("LIT");
	}
	else{
		kname = string(valLex->tk_value.vStr);
	}
	ValSymTable dval = retSymb(kname);
	return dval.tipo;
}
ValSymTable StackM::getSymbLex(lexic_val_type *valLex)
{
	string kname = string(valLex->tk_value.vStr);
	if(valLex->type == TIPO_LIT)
	{
		switch (valLex->value_type)
		{
		case LIT_TIPO_INT:
			kname = string(to_string(valLex->tk_value.vInt));
			break;
		case LIT_TIPO_BOOL:
			kname = string(to_string(valLex->tk_value.vBool));
			break;
		case LIT_TIPO_CHAR:
			kname = string(valLex->tk_value.vChar);
			break;
		case LIT_TIPO_FLOAT:
			kname = string(to_string(valLex->tk_value.vFloat));
			break;
		case LIT_TIPO_STRING:
			kname = string(valLex->tk_value.vStr);
			break;
		default:
			kname = string(valLex->tk_value.vStr);
			break;
		}
		kname.append("LIT");
	}
	ValSymTable dval = retSymb(kname);
	return dval;
}

int StackM::avalIn(lexic_val_type *valLex)
{
	int tipo = this->getTypeLex(valLex);
	if(tipo != ID_INT && tipo != ID_FLOAT )
	{
		string msg = "";
		string name = "";
		this->tError(ERR_WRONG_PAR_INPUT,valLex->lineno,name,msg);
	}
	return tipo;
}
int StackM::avalOut(lexic_val_type *valLex)
{

	int tipo = this->getTypeLex(valLex);
	if(tipo != ID_INT && tipo != ID_FLOAT )
	{
		string msg = "";
		string name = "";
		this->tError(ERR_WRONG_PAR_OUTPUT,valLex->lineno,name,msg);
	}
	return tipo;
}

void StackM::avalShift(lexic_val_type *valLex)
{
	if(valLex->tk_value.vInt > 16)
	{
		string msg =to_string(valLex->tk_value.vInt),name ="";
		this->tError(ERR_WRONG_PAR_SHIFT,valLex->lineno,name,msg);
	}

}

void StackM::updateTS(int tipo)
{
	SymMap::iterator itMapa;
	SymMap mapa = this->circ.back().getTable();
	for(itMapa = mapa.begin(); itMapa != mapa.end();++itMapa)
	{
		
		if((*itMapa).second.tipo == INDEF)
		{
			if(tipo == ID_STRING && (*itMapa).second.sect ==NAT_VET)
			{
				this->tError(ERR_STRING_VECTOR,(*itMapa).second.line,(*itMapa).first,"");
			} 
			this->circ.back().setByName((*itMapa).first, tipo, (*itMapa).second.tam*getSizeT(tipo));
			
		}
			
	}
}
void StackM::addSymA(string name, ValSymTable newSymbol)
{
	this->circ.back().insertSym(name, newSymbol);
}
bool StackM::checkSym(string name)
{
	for(StackMaps::reverse_iterator contexto = this->circ.rbegin();contexto !=this->circ.rend();++contexto )
	{
		if((*contexto).checkForSym(name))
			return true;
	}
	return false;
}
ValSymTable StackM::retSymb(string name)
{
	bool exitste = false;
	ValSymTable dadoRet;

	for(StackMaps::reverse_iterator contexto = this->circ.rbegin();contexto !=this->circ.rend();++contexto )
	{
		if((*contexto).checkForSym(name))
		{
			exitste = true;
			dadoRet = (*contexto).getTable()[name];
			break;
		}
			
	}

	return dadoRet;
}

void StackM::verifyRet(int tipoRet,int line)
{

	int tipoFunc =this->getTypeF();
	if(tipoRet!=tipoFunc && !(testVi(tipoRet,tipoFunc)))
	{
		string msg ="",name="";
		this->tError(ERR_WRONG_PAR_RETURN,line,name,msg);
	}
}
int StackM::getTypeF()
{
	ValSymTable dval = (*(this->circ.front().getTable().begin())).second;
	return dval.tipo;
}
void StackM::verifyAt(a_node* nodoDst,a_node* nodoOrig)
{
	string name = string(nodoDst->lexValue->tk_value.vStr);
	int line = nodoDst->lexValue->lineno;

	if(nodoOrig->typeSemVal == ID_CHAR && nodoDst->typeSemVal != ID_CHAR )
	{
		string msg = name;
		name = getNamelv(nodoOrig->lexValue);
		this->tError(ERR_CHAR_TO_X,line,name,msg);
	}

	if(nodoOrig->typeSemVal == ID_STRING && nodoDst->typeSemVal != ID_STRING )
	{
		string msg;
		msg = name;
		name = getNamelv(nodoOrig->lexValue);
		this->tError(ERR_STRING_TO_X,line,name,msg);
	}

	if(nodoDst->typeSemVal!=nodoOrig->typeSemVal && !(testVi(nodoOrig->typeSemVal,nodoDst->typeSemVal)))
	{
		string msg;
		msg= getNamelv(nodoOrig->lexValue);
		this->tError(ERR_WRONG_TYPE,line,name,msg);
	}

	if(nodoDst->typeSemVal == ID_STRING && nodoOrig->typeSemVal == ID_STRING )
	{
		ValSymTable dadoDst,dadoOrig;
		dadoDst = this->retSymb(string(nodoDst->lexValue->tk_value.vStr));
		dadoOrig = this->retSymb(string(nodoOrig->lexValue->tk_value.vStr));
		if(dadoOrig.tam > dadoDst.tam)
		{
			string msg;
			msg= "exceeded maximum size of "+ to_string(dadoDst.tam) + " received value with size " +to_string(dadoOrig.tam)+".";
			this->tError(ERR_STRING_MAX,line,name,msg);
		}
	}
}

ValSymTable StackM::getSym(string name)
{
	SymMap mapa = this->circ.back().getTable();
	return mapa[name];
}
void StackM::tError(int tipoErro,int line, string name,string aux)
{
	ValSymTable declaAnterior;
	switch (tipoErro)
	{
	case ERR_DECLARED:
		cout << "Identifier " << name << " in line " << line << " was already declared - "; 
		declaAnterior = getSym(aux);
		cout << "Previous declaration was at line " << declaAnterior.line << "." << endl;
		break;
	case ERR_FUNCTION:
		cout << "Function " << name << " was used as " << aux << " in line " << line << "."<<endl;
		break;
	case ERR_VECTOR:
		cout << "Vector " << name << " was used as " << aux << " in line " << line << "." << endl;
		break;
	case ERR_VARIABLE:
		cout << "Variable " << name << " was used as " << aux << " in line " << line << "."<<endl;
		break;
	case ERR_EXCESS_ARGS:
		cout << "Exceeded number of arguments to function call " << name <<" in line "<< line <<". " << aux << endl;
		break;
	case ERR_MISSING_ARGS:
		cout << "Missing arguments to function call " << name <<" in line "<< line <<". " <<aux << endl;
		break;
	case ERR_WRONG_TYPE_ARGS:
		cout << "Function " << name <<" in line " << line<< " " << aux << endl;
		break;
	case ERR_FUNCTION_STRING:
		cout << "Function " << name <<" in line " << line << " " << aux << endl;
		break;
	case ERR_UNDECLARED:
		cout << "Identifier " << name << " in line " << line << " was not declared before use" << endl;
		break; 
	case ERR_STRING_TO_X:
		cout << "Tried to make illegal conversion of string " << name << " to symbol " << aux<<" in line "<< line  << "." << endl;
		break;
	case ERR_CHAR_TO_X:
		cout << "Tried to make illegal conversion of char " << name << " to symbol " << aux <<" in line "<< line <<"." << endl;
		break;
	case ERR_WRONG_TYPE:
		cout << "Tried to make illegal conversion of " << name << " to symbol "<< aux <<" in line "<< line << "." << endl;
		break;
	case ERR_STRING_MAX:
		cout << "Assignment of string " << name << " in line " << line << " " << aux << "." << endl;
		break;
	case ERR_WRONG_PAR_INPUT:
		cout << "Used symbol of invalid type for input in line " << line << " expected type INT or FLOAT." << endl;
		break;
	case ERR_WRONG_PAR_OUTPUT:
		cout << "Used symbol of invalid type for output in line " << line << " expected type INT or FLOAT." << endl;
		break;
	case ERR_WRONG_PAR_SHIFT:
		cout << "Value " << aux << " exceeded limit of 16 for shift in line " << line << "." << endl;
		break;
	case ERR_WRONG_PAR_RETURN:
		cout << "Return value at line " << line << " does not implicitly match function type declaration."<< endl;
		break;
	case ERR_STRING_VECTOR:
		cout << "Declaring Vector " << name << " with illegal type STRING in line " << line << ".";
		break;	
	default:
		break;
	}

	exit(tipoErro);
}

