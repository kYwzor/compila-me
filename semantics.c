#include "semantics.h"


void print_tables(){
  printf("===== Global Symbol Table =====\n");
  Table_list aux = list;
  aux = aux->next;
  Sym_table aux_node = aux->table_node;
  if(aux != NULL){
    while(aux_node != NULL){
      char* s = get_label_string(aux_node->type);
      printf("%s\t%s\n", aux_node->name, s); 
      aux_node = aux_node->next;
    }
  }
  printf("\n");
  aux = list;
  aux = aux->next;
  while(aux != NULL){
    printf("===== Function %s Symbol Table =====\n", aux->table_node->name);
    aux_node = aux->table_node;
    printf("%s\t%s\n", "return", get_label_string(aux->table_node->type));
    Arg_list aux_args = aux->arg_list;
    while(aux_args != NULL){
      //TODO: complete this
      printf("%s\t%s/tparams\n", aux_args->name, get_label_string(aux_args->type)); 
    }
    aux_node = aux_node->next;
    while(aux_node != NULL){
      char* s = get_label_string(aux_node->type);
      printf("%s\t%s\n", aux_node->name, s); 
      aux_node = aux_node->next;
    }
    aux = aux->next;
    printf("\n");
  }
}

int handle_node(Node node){
  if(DEBUG) printf("Handling %s %s\n", node->value, get_label_string(node->label));
  switch(node->label){
    case Program:
      {
        list = (Table_list) malloc(sizeof(_table_list));
        Sym_table new_node = (Sym_table) malloc(sizeof(_sym_table));
        new_node->name = "Global";
        new_node->type = -1;
        new_node->next = NULL;
        list->table_node = new_node;
        list->next = NULL;

        current_table = new_node;
        global_table = new_node;
        full_expand(node);
        break;
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
          current_table->type = typeSpec->label;

        if(id->label != Id)
          return ERROR;
        else
          current_table->name = id->value;

        if(paramList->label != ParamList) return ERROR;
        if(funcBody->label != FuncBody) return ERROR;

        add_table(current_table);
        insert_symbol(global_table, current_table->name, current_table->type);

        if(node->child != NULL) 
          handle_node(node->child);
        current_table = global_table;
        if(node->brother != NULL) 
          handle_node(node->brother);
        break;
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


        Sym_table new_table = (Sym_table) malloc(sizeof(_sym_table));

        //Check if children are adequate
        if(typeSpec->label != Char && typeSpec->label != Double && typeSpec->label != Short && typeSpec->label != Int && typeSpec->label != Void){
          return ERROR;
        }
        else{
          new_table->type = typeSpec->label;
        }

        if(id->label != Id)
          return ERROR;
        else
          new_table->name = id->value;

        if(paramList->label != ParamList) return ERROR;
        add_table(new_table);

        full_expand(node);

        break;
      }

    case ParamList:
      full_expand(node);
      break;

    case ParamDeclaration: 
      {
        printf("%s is %s\n", get_label_string(node->type), get_label_string(ParamDeclaration));
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

        add_parameter(typeSpec, id);

        full_expand(node);
        break;
      }
  }
  return 1;
}

void add_table(Sym_table table){
  //Verificar se ja esta na tabela
  Table_list aux = list;
  while(aux->next != NULL){
    if(strcmp(aux->next->table_node->name, table->name) == 0)
      return;
    aux = aux->next;
  }
  Table_list new_node = (Table_list) malloc(sizeof(_table_list));
  new_node->next = NULL;
  new_node->table_node = table;
  new_node->arg_list = NULL;
  aux->next = new_node;
  if(DEBUG) printf("Adding a new symbol table: %s\n", new_node->table_node->name);
}

void add_parameter(Node type_spec, Node id){
  Table_list aux = list;
  printf("1\n");
  while(strcmp(aux->table_node->name, current_table->name) != 0){
    aux = aux->next;
  }
  printf("2\n");
  Arg_list args = aux->arg_list;
  while(args->next != NULL)
    args = args->next;
  printf("3\n");
  Arg_list new_arg= (Arg_list) malloc(sizeof(_arg_list));
  new_arg->type = type_spec->type;
  printf("4\n");
  if(id != NULL) new_arg->name = id->value;
  new_arg->next = NULL;
  printf("5\n");
  args->next = new_arg;
}

char* get_label_string(Label label){
  char* s;
  switch (label){
    case Program:
      s = "Program";
      break;
    case Declaration:
      s = "Declaration";
      break;
    case FuncDeclaration:
      s = "FuncDeclaration";
      break;
    case FuncDefinition:
      s = "FuncDefinition";
      break;
    case ParamList:
      s = "ParamList";
      break;
    case FuncBody:
      s = "FuncBody";
      break;
    case ParamDeclaration:
      s = "ParamDeclaration";
      break;
    case StatList:
      s = "StatList";
      break;
    case If:
      s = "If";
      break;
    case While:
      s = "While";
      break;
    case Return:
      s = "Return";
      break;
    case Or:
      s = "Or";
      break;
    case And:
      s = "And";
      break;
    case Eq:
      s = "Eq";
      break;
    case Ne:
      s = "Ne";
      break;
    case Lt:
      s = "Lt";
      break;
    case Gt:
      s = "Gt";
      break;
    case Le:
      s = "Le";
      break;
    case Ge:
      s = "Ge";
      break;
    case Add:
      s = "Add";
      break;
    case Sub:
      s = "Sub";
      break;
    case Mul:
      s = "Mul";
      break;
    case Div:
      s = "Div";
      break;
    case Mod:
      s = "Mod";
      break;
    case Not:
      s = "Not";
      break;
    case Minus:
      s = "Minus";
      break;
    case Plus:
      s = "Plus";
      break;
    case Store:
      s = "Store";
      break;
    case Comma:
      s = "Comma";
      break;
    case Call:
      s = "Call";
      break;
    case BitWiseAnd:
      s = "BitWiseAnd";
      break;
    case BitWiseXor:
      s = "BitWiseXor";
      break;
    case BitWiseOr:
      s = "BitWiseOr";
      break;
    case Char:
      s = "Char";
      break;
    case ChrLit:
      s = "ChrLit";
      break;
    case Id:
      s = "Id";
      break;
    case Int:
      s = "Int";
      break;
    case Short:
      s = "Short";
      break;
    case IntLit:
      s = "IntLit";
      break;
    case Double:
      s = "Double";
      break;
    case RealLit:
      s = "RealLit";
      break;
    case Void:
      s = "Void";
      break;
    case Null:
      s = "Null";
      break;
  }
  return s;
}

void full_expand(Node node){
  if(node->child != NULL)
    handle_node(node->child);
  if(node->brother != NULL)
    handle_node(node->brother);
}