#include "semantics.h"

void print_tables()
{
  printf("===== Global Symbol Table =====\n");
  Table_list aux = global_table;
  Sym_list global_aux = global_table->table_node->next;
  while (global_aux != NULL)
  {
    char *s = get_string_for_tables(global_aux->label);
    Arg_list aux_args = get_function_args(global_aux->name);
    if (aux_args == NULL)
    {
      printf("%s\t%s\n", global_aux->name, s);
    }
    else
    {
      printf("%s\t%s(", global_aux->name, s);
      while (aux_args != NULL)
      {
        printf("%s", get_string_for_tables(aux_args->label));
        aux_args = aux_args->next;
        if (aux_args != NULL)
          printf(",");
      }
      printf(")\n");
    }
    global_aux = global_aux->next;
  }
  aux = aux->next;
  printf("\n");
  aux = global_table;
  aux = aux->next;
  while (aux != NULL)
  {
    if (aux->is_defined)
    {
      printf("===== Function %s Symbol Table =====\n", aux->table_node->name);
      Sym_list aux_node = aux->table_node;
      printf("%s\t%s\n", "return", get_string_for_tables(aux->table_node->label));
      Arg_list aux_args = aux->arg_list;
      while (aux_args != NULL)
      {
        if (aux_args->label != Void && aux_args->name != NULL)
          printf("%s\t%s\tparam\n", aux_args->name, get_string_for_tables(aux_args->label));
        aux_args = aux_args->next;
      }
      aux_node = aux_node->next;
      while (aux_node != NULL)
      {
        char *s = get_string_for_tables(aux_node->label);
        printf("%s\t%s\n", aux_node->name, s);
        aux_node = aux_node->next;
      }
      printf("\n");
    }
    aux = aux->next;
  }
}

int handle_node(Node node)
{
  if (DEBUG)
    printf("\tHandling %s %s\n", node->value, get_label_string(node->label));

  switch (node->label)
  {
    case Program:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(Program));
      global_table = (Table_list)malloc(sizeof(_table_list));
      Sym_list new_node = (Sym_list)malloc(sizeof(_Sym_list));
      new_node->name = "Global";
      new_node->label = -1;
      new_node->next = NULL;
      global_table->table_node = new_node;
      global_table->next = NULL;

      current_table = global_table;

      Node aux = (Node) malloc(sizeof(Node_t)); //paramlist
      aux->child = (Node) malloc(sizeof(Node_t)); //ParamDeclaration
      aux->child->brother = NULL;
      Node type_spec = (Node)malloc(sizeof(Node_t));
      type_spec->label = Int;
      Node id = (Node)malloc(sizeof(Node_t));
      id->value = "a";
      aux->child->child = type_spec;
      type_spec->brother = id;
      create_function_entry("putchar", Int, aux);

      type_spec->label = Void;
      id->value = NULL;
      create_function_entry("getchar", Int, aux);

      free(id);
      free(type_spec);
      free(aux->child);
      free(aux);

      full_expand(node);
      break;
    }

    case FuncDefinition:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDefinition));
      Node type_spec = node->child;
      Node id = type_spec->brother;
      Node paramList = id->brother;

      current_table = find_function_entry(id->value);
      // TODO: We'll probably need to check if we aren't re-defining
      if (current_table == NULL)
        current_table = create_function_entry(id->value, type_spec->label, paramList);  

      handle_node(paramList->brother); //FuncBody
      current_table->is_defined = 1;

      current_table = global_table;

      if (node->brother != NULL)
        handle_node(node->brother);

      break;
    }

    case FuncDeclaration:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDeclaration));
      Node type_spec = node->child;
      Node id = type_spec->brother;
      Node paramList = id->brother;

      // TODO: We'll need to do something about re-declaring (else)
      if (find_function_entry(id->value) == NULL)
        create_function_entry(id->value, type_spec->label, paramList); 

      if (node->brother != NULL)
        handle_node(node->brother);

      break;
    }

    case Declaration:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(Declaration));
      Node type_spec = node->child;
      Node id = type_spec->brother;
      insert_symbol(current_table->table_node, id->value, type_spec->label);
      full_expand(node);
      break;
    }

    case FuncBody:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncBody));
      full_expand(node);
      break;
    }

    /* Statements */
    case StatList:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(StatList));
      full_expand(node);
      break;
    }

    case If:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(If));
      full_expand(node);
      break;
    }

    case While:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(While));
      full_expand(node);
      break;
    }
    case Return:
    {
      if (DEBUG)
        printf("%s is %s\n", get_label_string(node->label), get_label_string(Return));
      full_expand(node);
      break;
    }

    /* All operators, terminals and Null are defaulted for now */
    /* ParamList and ParamDeclaration are also defaulted, but they should never occur*/
    default:
      if (DEBUG)
        printf("Defaulted %s\n", get_label_string(node->label));
      full_expand(node);
      break;
  }
  return 1;
}

char *get_label_string(Label label)
{
  char *s;
  switch (label)
  {
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

void full_expand(Node node)
{
  if (node->child != NULL)
    handle_node(node->child);
  if (node->brother != NULL)
    handle_node(node->brother);
}

char *get_string_for_tables(Label label)
{
  char *s;
  switch (label)
  {
  case Char:
    s = "char";
    break;
  case Int:
    s = "int";
    break;
  case Short:
    s = "short";
    break;
  case Double:
    s = "double";
    break;
  case Void:
    s = "void";
  case Program:
    s = "void"; //hol up this is weird
    break;
  default:
    s = "\n\n\nTHIS SHOULD NEVER HAPPEN!\n\n\n";
    printf("%s", s);
    break;
  }
  return s;
}

Arg_list get_function_args(char *name)
{
  Table_list aux = global_table;
  while (aux != NULL)
  {
    if (strcmp(aux->table_node->name, name) == 0)
    {
      return aux->arg_list;
    }
    aux = aux->next;
  }
  return NULL;
}

Table_list find_function_entry(char* name)
{
  Table_list aux = global_table;
  while (aux != NULL) {
    if (strcmp(aux->table_node->name, name) == 0)
      return aux;
    aux = aux->next;
  }
  return NULL;
}

Table_list create_function_entry(char* name, Label label, Node paramList)
{
  // Assumes it's not already on the table
  Table_list aux = global_table;
  while (aux->next != NULL)
    aux = aux->next;

  Table_list new_node = (Table_list)malloc(sizeof(_table_list));
  new_node->next = NULL;
  new_node->is_defined = 0; // assumes not defined, easy to change on what is returned
  

  Sym_list new_list = (Sym_list)malloc(sizeof(_Sym_list));
  new_list->label = label;
  new_list->name = name;
  new_node->table_node = new_list;

  // A bit messy, but it adds all the arguments at once, if there are any
  // Might refactor later, but I think it works anyway
  Node paramDec = paramList->child;
  Node type_spec = paramDec->child;
  Node id = type_spec->brother;

  Arg_list args = (Arg_list)malloc(sizeof(_arg_list));
  args->label = type_spec->label;

  if (id != NULL)
    args->name = id->value;
  else
    args->name = NULL;

  args->next = NULL;

  new_node->arg_list = args;

  paramDec = paramDec->brother;

  while(paramDec!=NULL){
    type_spec = paramDec->child;
    id = type_spec->brother;

    Arg_list new_arg = (Arg_list)malloc(sizeof(_arg_list));
    new_arg->label = type_spec->label;

    if (id != NULL)
      new_arg->name = id->value;
    else
      new_arg->name = NULL;
      
    new_arg->next = NULL;
    args->next = new_arg;
    args = new_arg;
    paramDec = paramDec->brother;
  }         

  if (DEBUG)
    printf("Adding a new symbol table: %s\n", new_node->table_node->name);
  
  aux->next = new_node;
  insert_symbol(global_table->table_node, name, label);

  return new_node;
}