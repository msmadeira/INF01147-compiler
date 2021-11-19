/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include <iostream>
#include "stackB.hpp"
extern "C" int yyparse(void);
extern "C" int yylex_destroy(void);
void *tree = NULL;
StackM tabelas;

int main (int argc, char **argv)
{
  int ret = yyparse();
  exporta ((a_node*)tree);
  libera((a_node*)tree);
  tree = NULL;
  yylex_destroy();
  return ret;
}