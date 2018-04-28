#include "sym_table.h"

int insert_symbol(Sym_table table, char* name, Label type){
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

  Sym_table new_node = (Sym_table)malloc(sizeof(_sym_table));
  new_node->name = name;
  new_node->type = type;
  new_node->next = NULL;
  table->next = new_node;
  if(DEBUG) printf("Adding %s to %s table\n", new_node->name, table_name);
  return 1;
}

char* find_function_entry(char* id){
  //TODO
  return NULL;
}
