#include "semantics.h"

/*
keep in mind:

undef levanta sempre erro quando e operado
todos os tipos sao compativeis e guardamos o mais alto

declarations podem ser duplicadas na global mas nao dentro de funcoes

plus deve ser sempre inteiro? tricky shit

store fica com o tipo do ID
*/
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

    Node aux = (Node)malloc(sizeof(Node_t));   //paramlist
    aux->child = (Node)malloc(sizeof(Node_t)); //ParamDeclaration
    aux->child->brother = NULL;
    Node type_spec = (Node)malloc(sizeof(Node_t));
    type_spec->label = Int;
    aux->child->child = type_spec;
    type_spec->brother = NULL;
    create_function_entry("putchar", Int, aux, 0);

    type_spec->label = Void;
    create_function_entry("getchar", Int, aux, 0);

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
    if (current_table == NULL)
    {
      current_table = create_function_entry(id->value, type_spec->label, paramList, 1);
    }
    else
    {
      // Ja foi declarada, temos que verificar parametros
      Arg_list aux = current_table->arg_list;
      Node paramDec = paramList->child;
      while (paramDec != NULL)
      {
        Node type_spec = paramDec->child;
        Node id = type_spec->brother;
        if (DEBUG)
        {
          if (id != NULL)
            printf("Got label %s and name %s\n", get_label_string(type_spec->label), id->value);
          else
            printf("Got label %s and name %s\n", get_label_string(type_spec->label), NULL);
        }
        if (aux == NULL)
        {
          if (DEBUG)
            printf("error: there's more parameters on definition than on declaration\n");
          break;
        }
        else
        {
          if (DEBUG)
            printf("Existing label is %s\n", get_label_string(aux->label));
        }
        if (type_spec->label == aux->label)
        {
          if (id != NULL)
            aux->name = id->value;
        }
        else
        {
          if (DEBUG)
            printf("error: labels don't match\n");
        }

        paramDec = paramDec->brother;
        aux = aux->next;
      }

      if (aux != NULL)
      {
        if (DEBUG)
          printf("error: there's more parameters on declaration than on definition\n");
      }
      current_table->is_defined = 1;
    }

    handle_node(paramList->brother); //FuncBody

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
      create_function_entry(id->value, type_spec->label, paramList, 0);

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
    Node aux = id->brother;
    insert_symbol(current_table, id->value, type_spec->label);
    while(aux != NULL){
      put_type(aux);
      aux = aux->brother;
    }
    full_expand(node);
    break;
  }

  case RealLit:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(RealLit));
    node->type = Double;
    full_expand(node);
    break;
  }

  case IntLit:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(IntLit));
    node->type = Int;
    full_expand(node);
    break;
  }

  case ChrLit:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(ChrLit));
    node->type = Int;
    full_expand(node);
    break;
  }
  case Or:
  case And:
  case Eq:
  case Ne:
  case Lt:
  case Le:
  case Gt:
  case Ge:
  case Mod:
  {
    handle_node(node->child);
    put_type(node->child);
    put_type(node->child->brother);
    node->type = Int;
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    if (node->brother != NULL)
      handle_node(node->brother);
    break;
  }
  case Comma:
  {
    handle_node(node->child);
    put_type(node->child);
    put_type(node->child->brother);
    if(node->child->type == undef){
      node->type = undef; 
    }
    else{
      node->type = node->child->brother->type; 
    }
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case Add:
  case Sub:
  case Mul:
  case Div:
  case BitWiseAnd:
  case BitWiseXor:
  case BitWiseOr:
  {
    handle_node(node->child);
    put_type(node->child);
    put_type(node->child->brother);
    node->type = resolve_type(node->child->type, node->child->brother->type);
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case Store:
  {
    handle_node(node->child);
    put_type(node->child);
    put_type(node->child->brother);
    node->type = node->child->type;
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case Not:
  {
    handle_node(node->child);
    put_type(node->child);
    node->type = Int;
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }  
  case Minus:
  case Plus:
  {
    handle_node(node->child);
    put_type(node->child);
    node->type = node->child->type;
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case While:
  case Return:
  case If:
  {
    handle_node(node->child);
    put_type(node->child);
    Node aux = node->child->brother;
    while (aux != NULL)
    {
      put_type(aux);
      aux = aux->brother;
    }
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case Call:
  {
    handle_node(node->child);
    put_type(node->child);
    Node aux = node->child->brother;
    while (aux != NULL)
    {
      put_type(aux);
      aux = aux->brother;
    }
    node->type = node->child->type;
    if (node->brother != NULL)
      handle_node(node->brother);
    if (DEBUG)
      printf("Assigned %s to %s\n", get_label_string(node->label), get_label_string(node->type));
    break;
  }
  case FuncBody:
  {
    Node children = node->child;
    while(children != NULL){
      if(children->label == Id)
        put_type(children);
      children = children->brother;
    }
    full_expand(node);
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
    break;
  case undef:
    s = "undef";
    break;
  default:
    s = get_label_string(label);
    //printf("\n\n\nTHIS SHOULD NEVER HAPPEN!\n\n\n");
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

Table_list find_function_entry(char *name)
{
  if(DEBUG)printf("Looking for %s\n", name);
  if (name == NULL)
    return NULL;
  
  Table_list aux = global_table;
  while (aux != NULL)
  {
    if(DEBUG)printf("Comparing with %s\n", aux->table_node->name);
    if (strcmp(aux->table_node->name, name) == 0)
    {
      if(DEBUG)printf("Found %s\n", name);
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

Table_list create_function_entry(char *name, Label label, Node paramList, int is_definition)
{
  // Assumes it's not already on the table
  Table_list aux = global_table;
  while (aux->next != NULL)
    aux = aux->next;

  Table_list new_node = (Table_list)malloc(sizeof(_table_list));
  new_node->next = NULL;
  new_node->is_defined = is_definition;

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

  while (paramDec != NULL)
  {
    type_spec = paramDec->child;
    id = type_spec->brother;

    Arg_list new_arg = (Arg_list)malloc(sizeof(_arg_list));
    new_arg->label = type_spec->label;

    if (id != NULL && is_definition)
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
  insert_symbol(global_table, name, label);

  return new_node;
}
Label resolve_type(Label label1, Label label2)
{
  if (label1 == undef || label2 == undef)
  {
    return undef;
  }
  if (label1 == Double || label2 == Double)
  {
    return Double;
  }
  if (label1 == Int || label2 == Int)
  {
    return Int;
  }
  if (label1 == Short || label2 == Short)
  {
    return Short;
  }
  if (label1 == Char || label2 == Char)
  {
    return Char;
  }
  if (DEBUG)
    printf("There is a problem with the labels my dude\n");
  return Char;
}

void put_type(Node node)
{
  if (DEBUG)
    printf("Putting type for %s\n", get_label_string(node->label));
  switch (node->label)
  {
  case Id:
  {
    Arg_list arg_list = find_parameter(current_table, node->value);
    if (arg_list != NULL)
    {
      if (DEBUG)
        printf("%s encontrado na lista de argumentos\n", node->value);
      node->type = arg_list->label;
      return;
    }
    else
    {
      Sym_list symbol_entry = find_symbol(current_table, node->value);
      if (symbol_entry != NULL)
      {
        if (DEBUG){
          printf("%s encontrado na tabela local \n", node->value);
          printf("Symbol entry label is %s\n", get_label_string(symbol_entry->label));
        }
        node->type = symbol_entry->label;
      }
      else
      {
        symbol_entry = find_symbol(global_table, node->value);
        if (symbol_entry != NULL)
        {
          if (DEBUG)
            printf("%s encontrado na tabela global\n", node->value);
          node->type = symbol_entry->label;
          Table_list aux = find_function_entry(node->value);
          if (aux != NULL)
          {
            node->arg_list = aux->arg_list;
          }
        }
      }
      if(symbol_entry == NULL){
        node->type = undef;
      }
    }
    break;
  }
    case Void:
    case Char:
    case Double:
    case Int:
    case Short:
      node->type = node->label;
      break;
    default:
    {
      if (node->type == Empty)
      {
        /*THIS IS A HACK, O HOMEM DOS KEBABS SABERA COMO FAZER ISTO DECENTEMENTE
      printf("Putting type for %s\n", get_label_string(node->label));
      printf("---\nThis should never happen\n---\n");
      SE ELE NAO SOUBER VAMOS TENTAR FAZER ISTO EM HORAS MAIS NORMAIS*/
        node->type = Empty;
      }
      break;
    }
    }
}