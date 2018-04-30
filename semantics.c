#include "semantics.h"

void print_tables()
{
  printf("===== Global Symbol Table =====\n");
  Table_list aux = list;
  Sym_list global_aux = global_table->next;
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
  aux = list;
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
        if (aux_args->label != Void)
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
    list = (Table_list)malloc(sizeof(_table_list));
    Sym_list new_node = (Sym_list)malloc(sizeof(_Sym_list));
    new_node->name = "Global";
    new_node->label = -1;
    new_node->next = NULL;
    list->table_node = new_node;
    list->next = NULL;

    current_table = new_node;
    global_table = new_node;

    insert_symbol(new_node, "putchar", Int);
    Sym_list put_char_table = (Sym_list)malloc(sizeof(_Sym_list));
    put_char_table->label = Int;
    put_char_table->name = "putchar";
    put_char_table->next = NULL;
    current_table = put_char_table;
    Node type_spec = (Node)malloc(sizeof(Node_t));
    type_spec->label = Int;
    Node id = (Node)malloc(sizeof(Node_t));
    id->value = "a";
    add_table(put_char_table, 0);
    add_parameter(type_spec, id);

    insert_symbol(new_node, "getchar", Int);
    Sym_list get_char_table = (Sym_list)malloc(sizeof(_Sym_list));
    get_char_table->label = Int;
    get_char_table->name = "getchar";
    get_char_table->next = NULL;
    current_table = get_char_table;
    type_spec->label = Void;
    id->value = NULL;
    add_table(get_char_table, 0);
    add_parameter(type_spec, id);

    current_table = global_table;

    free(id);
    free(type_spec);
    full_expand(node);
    break;
  }

  case FuncDefinition:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDefinition));
    Node type_spec;
    Node id;
    Node paramList;

    type_spec = node->child;

    id = type_spec->brother;

    paramList = id->brother;

    current_table = find_function_entry(list, type_spec->label, id->value);

    insert_symbol(global_table, id->value, type_spec->label);

    if (node->child != NULL)
      handle_node(node->child);

    current_table = global_table;
    if (node->brother != NULL)
      handle_node(node->brother);
    break;
  }

  case FuncDeclaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDeclaration));
    Node type_spec;
    Node id;
    Node paramList;

    //Check if all the children exist
      type_spec = node->child;

      id = type_spec->brother;

      paramList = id->brother;

    current_table = (Sym_list)malloc(sizeof(_Sym_list));

    //Check if children are adequate
      current_table->label = type_spec->label;

      current_table->name = id->value;

    add_table(current_table, 0);
    insert_symbol(global_table, id->value, type_spec->label);

    if (node->child != NULL)
      handle_node(node->child);

    current_table = global_table;

    if (node->brother != NULL)
      handle_node(node->brother);

    break;
  }

  case ParamList:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(ParamList));
    full_expand(node);
    break;
  }

  case ParamDeclaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(ParamDeclaration));
    Node type_spec;
    Node id;

    //Check if param has type
      type_spec = node->child;

    if (type_spec->brother != NULL)
      id = type_spec->brother;
    else
      id = NULL;

    add_parameter(type_spec, id);

    full_expand(node);
    break;
  }

  case Declaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Declaration));
    Node type_spec = node->child;
    Node id = type_spec->brother;
    insert_symbol(current_table, id->value, type_spec->label);
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
  case Or:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Or));
    full_expand(node);
    break;
  }
  case And:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(And));
    full_expand(node);
    break;
  }
  case Eq:
  {
    full_expand(node);
    break;
  }
  case Ne:
  {
    full_expand(node);
    break;
  }
  case Lt:
  {
    full_expand(node);
    break;
  }
  case Gt:
  {
    full_expand(node);
    break;
  }
  case Le:
  {
    full_expand(node);
    break;
  }
  case Ge:
  {
    full_expand(node);
    break;
  }
  case Add:
  {
    full_expand(node);
    break;
  }
  case Sub:
  {
    full_expand(node);
    break;
  }
  case Mul:
  {
    full_expand(node);
    break;
  }
  case Div:
  {
    full_expand(node);
    break;
  }
  case Mod:
  {
    full_expand(node);
    break;
  }
  case Not:
  {
    full_expand(node);
    break;
  }
  case Minus:
  {
    full_expand(node);
    break;
  }
  case Plus:
  {
    full_expand(node);
    break;
  }
  case Store:
  {
    full_expand(node);
    break;
  }
  case Comma:
  {
    full_expand(node);
    break;
  }
  case Call:
  {
    full_expand(node);
    break;
  }
  case BitWiseAnd:
  {
    full_expand(node);
    break;
  }
  case BitWiseXor:
  {
    full_expand(node);
    break;
  }
  case BitWiseOr:
  {
    full_expand(node);
    break;
  }
  case Char:
  {
    full_expand(node);
    break;
  }
  case ChrLit:
  {
    full_expand(node);
    break;
  }
  case Id:
  {
    full_expand(node);
    break;
  }
  case Int:
  {
    full_expand(node);
    break;
  }
  case Short:
  {
    full_expand(node);
    break;
  }
  case IntLit:
  {
    full_expand(node);
    break;
  }
  case Double:
  {
    full_expand(node);
    break;
  }
  case RealLit:
  {
    full_expand(node);
    break;
  }
  case Void:
  {
    full_expand(node);
    break;
  }
  case Null:
  {
    full_expand(node);
    break;
  }
  }
  return 1;
}

void add_table(Sym_list table, int is_defined)
{
  //Verificar se ja esta na tabela
  Table_list aux = list;
  while (aux->next != NULL)
  {
    if (strcmp(aux->next->table_node->name, table->name) == 0)
      return;
    aux = aux->next;
  }
  Table_list new_node = (Table_list)malloc(sizeof(_table_list));
  new_node->next = NULL;
  new_node->table_node = table;
  new_node->is_defined = is_defined;
  new_node->arg_list = NULL;
  aux->next = new_node;
  if (DEBUG)
    printf("Adding a new symbol table: %s\n", new_node->table_node->name);
}

void add_parameter(Node type_spec, Node id)
{
  Table_list aux = list;
  while (strcmp(aux->table_node->name, current_table->name) != 0)
  {
    if (DEBUG)
      printf("%s---%s\n", aux->table_node->name, current_table->name);
    aux = aux->next;
    if (DEBUG)
      printf("Problem in table node\n");
    Sym_list a = aux->table_node;
    if (DEBUG)
      printf("Problem in table node name\n");
    char *b = a->name;
    if (DEBUG)
      printf("Problem in table\n");
    b = current_table->name;
  }
  Arg_list args = aux->arg_list;
  if (args != NULL)
  {
    while (args->next != NULL)
      args = args->next;
  }
  Arg_list new_arg = (Arg_list)malloc(sizeof(_arg_list));
  new_arg->label = type_spec->label;
  if (id != NULL)
    new_arg->name = id->value;
  new_arg->next = NULL;
  if (args != NULL)
    args->next = new_arg;
  else
    aux->arg_list = new_arg;
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
  char *s = NULL;
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
    s = "void";
    break;
  default:
    printf("never ever pc cucks");
    break;
  }
  return s;
}

Arg_list get_function_args(char *name)
{
  Table_list aux = list;
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

Sym_list find_function_entry(Table_list list, Label label, char *value)
{
  Table_list aux = list;
  while (aux != NULL)
  {
    if (strcmp(aux->table_node->name, value) == 0)
    {
      aux->is_defined = 1;
      return aux->table_node;
    }
    aux = aux->next;
  }
  Sym_list new_list = (Sym_list)malloc(sizeof(_Sym_list));
  new_list->label = label;
  new_list->name = value;
  add_table(new_list, 1);
  return new_list;
}