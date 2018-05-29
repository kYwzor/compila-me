#include "generation.h"
char *return_value;
void generate_code(Node node)
{
  if (DEBUG)
    printf("\tHandling %s %s\n", node->value, get_label_string(node->label));

  switch (node->label)
  {
  case Program:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Program));
    full_generation(node);
    break;
  }

  case FuncDefinition:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDefinition));
    Node type_spec = node->child;
    Node id = type_spec->brother;
    Node paramList = id->brother;

    printf("define %s @%s(){\n", get_llvm_type(type_spec->label), id->value);
    if (node->child != NULL)
      generate_code(node->child);

    printf("ret %s %s\n}\n", get_llvm_type(type_spec->label), handle_constant(type_spec->label, return_value));
    return_value = NULL;
    if (node->brother != NULL)
      generate_code(node->brother);

    break;
  }

  case FuncDeclaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDeclaration));
    full_generation(node);
    break;
  }

  case Declaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Declaration));
    full_generation(node);
    break;
  }

  case Return:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Return));
    return_value = node->child->value;
    full_generation(node);
  }

  /* All operators, terminals and Null are defaulted for now */
  /* ParamList and ParamDeclaration are also defaulted, but they should never occur*/
  default:
    if (DEBUG)
      printf("Defaulted %s\n", get_label_string(node->label));
    full_generation(node);
    break;
  }
}

void full_generation(Node node)
{
  if (node->child != NULL)
    generate_code(node->child);
  if (node->brother != NULL)
    generate_code(node->brother);
}

char *get_llvm_type(Label label)
{
  char *s = NULL;
  switch (label)
  {
  /*TODO: estes dois sao gerados com signext antes do tamanho, verificar se e necessario*/
  case Char:
    s = "i8";
    break;
  case Short:
    s = "i16";
    break;
  case Int:
    s = "i32";
    break;
  case Double:
    s = "double";
    break;
  case Void:
    s = "void";
    break;
  default:
    printf("FATAL: Invalid llvm type\n");
  }
  return s;
}
char *get_label_string(Label label)
{
  char *s;
  switch (label)
  {
  case Empty:
    s = "Empty";
    break;
  case undef:
    s = "undef";
    break;
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

char *handle_constant(Label type, char *value)
{
  char *s = NULL;
  int octal_flag = 0;
  int i = 0;
  switch (type)
  {
  case Double:
  case Short:
    /*
    while(value[i] != '\0'){
      if(value[i] == 'e' || value[i] == "E"){
        exponential_flag = 1;
      }
    }
    if(exponential_flag){
      double aux;
      sscanf(value, "%lf", &aux);
    }
    */
    s = value;
    break;
  case Int:
    if (value[0] == '0')
    {
      int aux_int;
      char aux_str[1024];
      sscanf(value, "%o", &aux_int);
      sprintf(aux_str, "%d", aux_int);
      printf("%s\n", aux_str);
      s = aux_str;
    }
    else
    {
      s = value;
    }
    break;
  case Char:{
    int aux_int;
    char aux_str[1024];
    //TODO: Temos de fazer um caso especial para os caracteres \t \n e assim
    aux_int = value[1];
    sprintf(aux_str, "%d", aux_int);
    s = aux_str;
    break;
  }
  case Void:
    s = "";
    break;
  default:
    printf("FATAL: Invalid constant type\n");
  }
  return s;
}