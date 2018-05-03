#include "sym_list.h"

int insert_symbol(Table_list table, char* name, Label label){
  //Nao protegido para table == null
  Sym_list sym_list = table->table_node;
  Arg_list arg_list = table->arg_list;

  char* table_name = sym_list->name;

  while(arg_list != NULL){
    if(arg_list->name != NULL){
      if(strcmp(arg_list->name, name) == 0){
        //-1 means error
        return -1;
      }
    }
    arg_list = arg_list->next;
  }
  while(sym_list->next != NULL){
    sym_list = sym_list->next;
    if(strcmp(sym_list->name, name) == 0){
      //-1 means error
      return -1;
    }
  }

  Sym_list new_node = (Sym_list)malloc(sizeof(_Sym_list));
  new_node->name = name;
  new_node->label = label;
  new_node->next = NULL;
  sym_list->next = new_node;
  if(DEBUG) printf("Adding %s to %s table\n", new_node->name, table_name);
  return 1;
}

Sym_list find_symbol(Table_list table_list, char* name){
  Sym_list aux = table_list->table_node->next;  //salta o primeiro elemento, return do nome da funcao
  while(aux != NULL){
    //Isto nao esta protegido para segfault mas acho que nao e preciso
    if(strcmp(aux->name, name) == 0){
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

Arg_list find_parameter(Table_list table_list, char* name){
  Arg_list aux = table_list->arg_list;
  while(aux != NULL){
    //Isto nao esta protegido para segfault mas acho que nao e preciso
    if(aux->name != NULL)
      if(strcmp(aux->name, name) == 0){
        return aux;
      }
    aux = aux->next;
  }
  return NULL;
}