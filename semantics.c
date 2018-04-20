#include "semantics.h"

#define ERROR -1


void semantic_analysis(Node node){
  handle_node(node);
  if(node->child != NULL)
    semantic_analysis(node->child);
}

void print_tables(){
  while(list != NULL){
    printf("===== Function %s Symbol Table =====\n", list->table_node->name);
    while(list->table_node != NULL){
      char* s = get_label_string(list->table_node->type);
      printf("%s\t%s", list->table_node->name, s); 
    }
  }
}

int handle_node(Node node){
  switch(node->label){
    case Program:
      {
      list = (Table_list) malloc(sizeof(_table_list));
      Sym_table new_node = (Sym_table) malloc(sizeof(_sym_table));
      new_node->name = NULL;
      new_node->type = -1;
      new_node->next = NULL;
      list->table_node = new_node;
      list->next = NULL;
      
      current_table = new_node;
      }

    case FuncDefinition:
      {
      Node typeSpec;
      Node id;
      Node paramList;
      Node funcBody;

      //Check if all the children exist
      if(node->child != NULL)
        typeSpec = node->child;
      else
        return ERROR;

      if(typeSpec->brother != NULL)
        id = typeSpec->brother;
      else
        return ERROR;

      if(id->brother != NULL)
        paramList = id->brother;
      else
        return ERROR;

      if(paramList->brother != NULL)
        funcBody = paramList->brother;
      else
        return ERROR;


      current_table = (Sym_table) malloc(sizeof(_sym_table));

      //Check if children are adequate
      if(typeSpec->label != Char && typeSpec->label != Double && typeSpec->label != Short && typeSpec->label != Int && typeSpec->label != Void)
        return ERROR;
      else
        current_table->type = typeSpec->label;;

      if(id->label != Id)
        return ERROR;
      else
        current_table->name = id->value;

      if(paramList->label != ParamList) return ERROR;
      if(funcBody->label != FuncBody) return ERROR;
      add_table(current_table);
      }

    case FuncDeclaration:
      {
      Node typeSpec;
      Node id;
      Node paramList;

      //Check if all the children exist
      if(node->child != NULL)
        typeSpec = node->child;
      else
        return ERROR;

      if(typeSpec->brother != NULL)
        id = typeSpec->brother;
      else
        return ERROR;

      if(id->brother != NULL)
        paramList = id->brother;
      else
        return ERROR;


      current_table = (Sym_table) malloc(sizeof(_sym_table));

      //Check if children are adequate
      if(typeSpec->label != Char && typeSpec->label != Double && typeSpec->label != Short && typeSpec->label != Int && typeSpec->label != Void){
        return ERROR;
      }
      else{
        current_table->type = typeSpec->label;
      }

      if(id->label != Id)
        return ERROR;
      else
        current_table->name = id->value;

      if(paramList->label != ParamList) return ERROR;
      add_table(current_table);
      }

    case ParamDeclaration: 
      {
      Node typeSpec;
      Node id;

      //Check if param has type 
      if(node->child != NULL)
        typeSpec = node->child;
      else
        return ERROR;


      if(typeSpec->brother != NULL)
        id = typeSpec->brother;
      else
        id = NULL;
      }
  }
  return 1;
}

  void add_table(Sym_table table){
    //Verificar se ja esta na tabela
    Table_list aux = list;
    while(aux->next != NULL){
      aux = aux->next;
    }
    Table_list new_node = (Table_list) malloc(sizeof(_table_list));
    new_node->next = NULL;
    new_node->table_node = table;
    new_node->arg_list = NULL;
    aux->next = new_node;
  }

