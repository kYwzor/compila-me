#include "tree.h"
int insert_symbol(Symbol_table table, char* name, Label type){
  //Nao protegido para table == null
  while(table->next != NULL){
    table = table->next;
    if(strcmp(table->name, name) == 0){
      //-1 means error
      free(name);
      return -1;
    }
  }
  Sym_table new_node = (Sym_table)malloc(sizeof(_sym_table));
  new_node->name = name;
  new_node->type = type;
  new_node->next = NULL;
  table->next = new_node;
  return 1;
}
