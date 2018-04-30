#include "sym_list.h"

int insert_symbol(Sym_list table, char* name, Label label){
  //Nao protegido para table == null

  char* table_name = table->name;
  while(table->next != NULL){
    table = table->next;
    if(strcmp(table->name, name) == 0){
      //-1 means error
      free(name);
      return -1;
    }
  }

  Sym_list new_node = (Sym_list)malloc(sizeof(_Sym_list));
  new_node->name = name;
  new_node->label = label;
  new_node->next = NULL;
  table->next = new_node;
  if(DEBUG) printf("Adding %s to %s table\n", new_node->name, table_name);
  return 1;
}

char* find_function_entry(char* id){
  return NULL;
}

