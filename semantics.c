#include "tree.h"
#include "sym_table.h"
#define ERROR -1


void semantic_analysis(Node node){
  handle_node(node);
  if(node->child != NULL)
    semantic_analysis(node->child);
}

int handle_node(Node node){
  switch(node->label){
    case Program:
      list = (Table_list) malloc(sizeof(_table_list));
      Sym_table new_node = (Sym_table) malloc(sizeof(_sym_table));
      new_node->name = NULL;
      new_node->type = NULL;
      new_node->next = NULL;
      list->node = new_node;
      list->next = NULL;

    case FuncDefinition:
      Node typeSpec;
      Node id;
      Node paramList;
      Node funcBody;

      //Check if all the children exist
      node->child != NULL ? typeSpec = node->child : return ERROR;
      typeSpec->brother != NULL ? id = typeSpec->brother : return ERROR;
      id->brother != NULL ? paramList = id->brother : return ERROR;
      paramList->brother != NULL ? funcBody = paramList->brother : return ERROR;

      current_table = (Sym_table) malloc(sizeof(_sym_table));

      //Check if children are adequate
      typeSpec->label != Char && typeSpec->label != Double && typeSpec->label != Short && typeSpec->label != Int && typeSpec->label != Void ? return ERROR: current_table->type = typeSpec->label;
      id->label != Id ? return ERROR : current_table->name = id->value;
      if(paramList->label != ParamList) return ERROR;
      if(funcBody->label != FuncBody) return ERROR;
      add_table(current_table);

    case FuncDeclaration:
      Node typeSpec;
      Node id;
      Node paramList;

      //Check if all the children exist
      node->child != NULL ? typeSpec = node->child : return ERROR;
      typeSpec->brother != NULL ? id = typeSpec->brother : return ERROR;
      id->brother != NULL ? paramList = id->brother : return ERROR;

      current_table = (Sym_table) malloc(sizeof(_sym_table));

      //Check if children are adequate
      if(typeSpec->label != Char && typeSpec->label != Double && typeSpec->label != Short && typeSpec->label != Int && typeSpec->label != Void){
        return ERROR;
      }
      else{
        current_table->type = typeSpec->label;
      }

      id->label != Id ? return ERROR : current_table->name = id->value;
      if(paramList->label != ParamList) return ERROR;
      add_table(current_table);

    case ParamDeclaration: 
      Node typeSpec;
      Node id;

      //Check if param has type 
      node->child != NULL ? typeSpec = node->child : return ERROR;

      typeSpec->brother != NULL ? id = typeSpec->brother : id = NULL;
      //TODO: Acabar esta funcao, parece me que um parametro pode ou nao ter um id, agora e uma questao de acrescentar as cenas adequedamente.
  }

  void add_table(Sym_table table){
    //Verificar se ja esta na tabela
    Table_list aux = list;
    while(aux->next != NULL){
      aux = aux->next;
    }
    Table_list new_node = (Table_list) malloc(sizeof(_table_list));
    new_node->next = NULL;
    new_node->node = table;
    new_node->arg_list = NULL;
    aux->next = new_node;
  }

