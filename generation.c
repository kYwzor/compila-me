#include "generation.h"

int r_count = 1;

void generate_code(Node node)
{
  int aux1, aux2;
  if (DEBUG)

    printf("\tHandling %s %s\n", node->value, get_label_string(node->label));

  switch (node->label)
  {
  case Program:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Program));

    printf("declare i32 @putchar(i32)\n");
    printf("declare i32 @getchar()\n");

    full_generation(node);

    break;
  }

  case FuncDefinition:
  {
    r_count = 1;
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDefinition));
    Node type_spec = node->child;
    Node id = type_spec->brother;
    Node paramList = id->brother;

    printf("define %s @%s(){\n", get_llvm_type(type_spec->label), id->value);
    generate_code(paramList->brother);	//funcbody

    printf("ret %s %s\n}\n", get_llvm_type(type_spec->label), get_default_value(type_spec->label)); //return default, fica no final da funcao, provavelmente inalcancavel. Isto e suposto ser assim
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
    Node type_spec = node->child;
    Node id = type_spec->brother;
    
    printf("%%%s = alloca %s\n", id->value, get_llvm_type(type_spec->label));	//align???
    Node aux = id->brother;
    if (aux != NULL){
      generate_code(aux);
      printf("store %s %%%d, %s* %%%s\n", get_llvm_type(type_spec->label), r_count - 1, get_llvm_type(type_spec->label), id->value);
    }

    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  }

  case Return:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Return));
    if(node->child == NULL){
      printf("ret void\n");
      break;
    }
    generate_code(node->child);
    printf("ret %s %%%d\n", get_llvm_type(node->child->type), r_count - 1);

    break;
  }
  case Minus:
    generate_code(node->child);
    aux1 = r_count - 1;
    printf("%%%d = sub nsw %s 0, %%%d\n", r_count++, get_llvm_type(node->child->type), aux1);
    break;
  case Store:
    generate_code(node->child->brother);
    if(node->type == Char){
      printf("store i32 %%%d, i32* %%%s\n", r_count - 1, node->child->value);
    }
    else if(node->type == Short){
      printf("store i16 %%%d, i16* %%%s\n", r_count - 1, node->child->value);
    }
    else if(node->type == Int){
      printf("store i32 %%%d, i32* %%%s\n", r_count - 1, node->child->value);
    }
    else{
      printf("store double %%%d, double* %%%s\n", r_count - 1, node->child->value);
    }
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Add:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type == Int){
      printf("%%%d = add i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    else{
      //THIS SOULD ALWAYS BE DOUBLE
      if(node->child->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux1);
        aux1 = r_count - 1;
      }
      if(node->child->brother->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux2);
        aux2 = r_count - 1;
      }
      printf("%%%d = fadd double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Sub:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type == Int){
      printf("%%%d = sub i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    else{
      //THIS SOULD ALWAYS BE DOUBLE
      if(node->child->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux1);
        aux1 = r_count - 1;
      }
      if(node->child->brother->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux2);
        aux2 = r_count - 1;
      }
      printf("%%%d = fsub double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Mul:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type == Int){
      printf("%%%d = mul i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    else{
      //THIS SOULD ALWAYS BE DOUBLE
      if(node->child->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux1);
        aux1 = r_count - 1;
      }
      if(node->child->brother->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux2);
        aux2 = r_count - 1;
      }
      printf("%%%d = fmul double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Div:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type == Int){
      printf("%%%d = sdiv i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    else{
      //THIS SOULD ALWAYS BE DOUBLE
      if(node->child->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux1);
        aux1 = r_count - 1;
      }
      if(node->child->brother->type == Int){
        printf("%%%d = sitofp i32 %%%d to double\n", r_count++, aux2);
        aux2 = r_count - 1;
      }
      printf("%%%d = fdiv double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
    break;
  case RealLit:
    printf("%%%d = fadd double %s, %s\n", r_count++, get_default_value(Double), handle_constant(Double, node->value));
    // if (node->brother != NULL)
    //   generate_code(node->brother);
    break;
  case IntLit:
    printf("%%%d = add i32 %s, %s\n", r_count++, get_default_value(Int), handle_constant(Int, node->value));
    // if (node->brother != NULL)
    //   generate_code(node->brother);
    break;
  case ChrLit:
    printf("%%%d = add i32 %s, %s\n", r_count++, get_default_value(Char), handle_constant(Char, node->value));
    // isto esta provavelmente mal. Devo ter que fazer i8 se possivel ou conversao para i32 beforehand
    // if (node->brother != NULL)
    //   generate_code(node->brother);
    break;
  case Id:
    printf("%%%d = load %s, %s* %%%s\n", r_count++, get_llvm_type(node->type), get_llvm_type(node->type), node->value);
    // if (node->brother != NULL)
    //   generate_code(node->brother);
    break;
  case Call:
  { //verificar como funciona funcao sem argumentos
    Node aux = node->child->brother;
    char *param_string = (char *)malloc(sizeof(char) * 1024);
    strcpy(param_string, "");
    while (aux != NULL)
    {
      generate_code(aux);
      char *aux_string = (char *)malloc(sizeof(char) * 1024);
      if (param_string[0] != '\0')
        sprintf(aux_string, ", %s %%%d", get_llvm_type(node->type), r_count - 1);
      else
        sprintf(aux_string, "%s %%%d", get_llvm_type(node->type), r_count - 1);
      strcat(param_string, aux_string);
      aux = aux->brother;
    }
    if (node->type != Void)
      printf("%%%d = call %s @%s(%s", r_count++, get_llvm_type(node->type), node->child->value, param_string);
    else
      printf("call %s @%s(%s", get_llvm_type(node->type), node->child->value, param_string);
    printf(")\n");
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  }
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
    s = "i32";
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
    printf("FATAL: Invalid llvm type %s\n", get_label_string(label));
  }
  return s;
}

char *get_default_value(Label label)
{
  char *s = NULL;
  switch (label)
  {
  case Char:
    s = "0"; //not sure about this one
    break;
  case Short:
    s = "0"; //not sure about this one
    break;
  case Int:
    s = "0";
    break;
  case Double:
    s = "0.000000e+00";
    break;
  case Void:
    s = "";
    break;
  default:
    printf("FATAL: Invalid type for default\n");
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
  switch (type)
  {
  case Double:
  {
    double aux_double;
    char aux_str[1024]; // this seems dangerous to me... returning something created here...
    //printf("value %s\n", value);
    sscanf(value, "%lf", &aux_double);
    //printf("aux_double %lf\n", aux_double);
    sprintf(aux_str, "%.16E", aux_double); //verificar quantas casas devem ser
    //printf("aux_str %s\n", aux_str);
    s = aux_str;
    break;
  }
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
      char aux_str[1024]; // this seems dangerous to me... returning something created here...
      sscanf(value, "%o", &aux_int);
      sprintf(aux_str, "%d", aux_int);
      s = aux_str;
    }
    else
    {
      s = value;
    }
    break;
  case Char:
  {
    char aux_char = '\0';
    char aux_str[1024]; // this seems dangerous to me... returning something created here...
    //TODO: Temos de fazer um caso especial para os caracteres \t \n e assim
    //printf("value %s\n", value);
    if(value[3] != '\0'){
      if(value[2] == 'n'){
        aux_char = '\n';
      }
      else if(value[2] == 't'){
        aux_char = '\t';
      }
      else if(value[2] == '\\'){
        aux_char = '\\';
      }
      else if(value[2] == '\''){
        aux_char = '\'';
      }
      else if(value[2] == '"'){
        aux_char = '"';
      }
    }
    else{
    sscanf(value, "'%c'", &aux_char);	//thought this would work :(
    }
    //printf("aux_char %c\n", aux_char);
    sprintf(aux_str, "%d", aux_char);
    //printf("aux_str %s\n", aux_str);
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