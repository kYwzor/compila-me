#include "semantics.h"

void print_tables()
{
  printf("===== Global Symbol Table =====\n");
  printf("putchar\tint(int)\n");
  printf("getchar\tint(void)\n");
  Table_list aux = list;
  if (aux != NULL)
  {
    aux = aux->next;
    if (aux != NULL)
    {
      Sym_table aux_node = aux->table_node;
      while (aux != NULL)
      {
        Sym_table aux_node2 = aux->table_node;
        char *s = get_string_for_tables(aux_node2->label);
        printf("%s\t%s(", aux_node2->name, s);
        Arg_list aux_args = aux->arg_list;
        while (aux_args != NULL)
        {
          printf("%s", get_string_for_tables(aux_args->label));
          aux_args = aux_args->next;
          if (aux_args != NULL)
            printf(", ");
        }
        printf(")\n");
        aux = aux->next;
      }

      printf("\n");
      aux = list;
      aux = aux->next;
      while (aux != NULL)
      {
        printf("===== Function %s Symbol Table =====\n", aux->table_node->name);
        aux_node = aux->table_node;
        printf("%s\t%s\n", "return", get_string_for_tables(aux->table_node->label));
        Arg_list aux_args = aux->arg_list;
        while (aux_args != NULL)
        {
          if (aux_args->label != Void)
            printf("%s\t%s\tparams\n", aux_args->name, get_string_for_tables(aux_args->label));
          aux_args = aux_args->next;
        }
        aux_node = aux_node->next;
        while (aux_node != NULL)
        {
          char *s = get_string_for_tables(aux_node->label);
          printf("%s\t%s\n", aux_node->name, s);
          aux_node = aux_node->next;
        }
        aux = aux->next;
        printf("\n");
      }
    }
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
    Sym_table new_node = (Sym_table)malloc(sizeof(_sym_table));
    new_node->name = "Global";
    new_node->label = -1;
    new_node->next = NULL;
    list->table_node = new_node;
    list->next = NULL;

    current_table = new_node;
    global_table = new_node;
    /*TODO: Ver isto depois com o Tiago, print ficou hardcoded
         * para encontrar o print faz-se search de foobar
         insert_symbol(new_node, "putchar", Int);
         insert_symbol(new_node, "getchar", Int);
         */
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
    Node funcBody;

    //Check if all the children exist
    if (node->child != NULL)
      type_spec = node->child;
    else
      return ERROR;

    if (type_spec->brother != NULL)
      id = type_spec->brother;
    else
      return ERROR;

    if (id->brother != NULL)
      paramList = id->brother;
    else
      return ERROR;

    if (paramList->brother != NULL)
      funcBody = paramList->brother;
    else
      return ERROR;

    current_table = (Sym_table)malloc(sizeof(_sym_table));

    //Check if children are adequate
    if (type_spec->label != Char && type_spec->label != Double && type_spec->label != Short && type_spec->label != Int && type_spec->label != Void)
      return ERROR;
    else
      current_table->label = type_spec->label;

    if (id->label != Id)
      return ERROR;
    else
      current_table->name = id->value;

    if (paramList->label != ParamList)
      return ERROR;
    if (funcBody->label != FuncBody)
      return ERROR;

    add_table(current_table);
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
    if (node->child != NULL)
      type_spec = node->child;
    else
      return ERROR;

    if (type_spec->brother != NULL)
      id = type_spec->brother;
    else
      return ERROR;

    if (id->brother != NULL)
      paramList = id->brother;
    else
      return ERROR;

    Sym_table new_table = (Sym_table)malloc(sizeof(_sym_table));

    //Check if children are adequate
    if (type_spec->label != Char && type_spec->label != Double && type_spec->label != Short && type_spec->label != Int && type_spec->label != Void)
    {
      return ERROR;
    }
    else
    {
      new_table->label = type_spec->label;
    }

    if (id->label != Id)
      return ERROR;
    else
      new_table->name = id->value;

    if (paramList->label != ParamList)
      return ERROR;
    add_table(new_table);
    insert_symbol(global_table, id->value, type_spec->label);

    full_expand(node);

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
    if (node->child != NULL)
      type_spec = node->child;
    else
      return ERROR;

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

void add_table(Sym_table table)
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
    aux = aux->next;
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
    s = "void";
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
  case ChrLit:
    s = "ChrLit";
    break;
  case Id:
    s = "Id";
    break;
  case IntLit:
    s = "IntLit";
    break;
  case RealLit:
    s = "RealLit";
    break;
  case Null:
    s = "Null";
    break;
  }
  return s;
}