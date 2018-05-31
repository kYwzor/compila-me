#include "generation.h"

// TODO: REMEMBER TO ALSO CHECK FOR INT CONVERSIONS ON OPERATIONS
// IT'S POSSIBLE THAT WE HAVE AN ID
// ALSO CHECK IF TWO CHAR IDS CAN MAKE A CHAR ADD

int r_count = 1;
Label current_function_type = -1;
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
    current_function_type = type_spec->label;

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
      aux1 = convert_register(type_spec->label, aux->type, r_count -1);
      printf("store %s %%%d, %s* %%%s\n", get_llvm_type(type_spec->label), aux1, get_llvm_type(type_spec->label), id->value);
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
    aux1 = convert_register(current_function_type, node->child->type, r_count -1);
    printf("ret %s %%%d\n", get_llvm_type(node->child->type), aux1);

    break;
  }

  case Store:
    generate_code(node->child->brother);
    aux1 = convert_register(node->type, node->child->brother->type, r_count -1);
    printf("store %s %%%d, %s* %%%s\n", get_llvm_type(node->type), aux1, get_llvm_type(node->type), node->child->value);
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  /*
  case Or:
  case And:
  */
  case Eq:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp eq i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp oeq double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Ne:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp ne i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp une double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Lt:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp slt i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp olt double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Gt:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp sgt i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp ogt double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Le:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp sle i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp ole double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  case Ge:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double)){
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("%%%d = icmp sge i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fcmp oge double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Add:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type != Double){
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("%%%d = add %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
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

    if (node->type != Double){
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("%%%d = sub %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
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

    if (node->type != Double){
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("%%%d = mul %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
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

    if (node->type != Double){
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("%%%d = sdiv %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("%%%d = fdiv double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Mod:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;
    aux1 = convert_register(Int, node->child->type, aux1);
    aux2 = convert_register(Int, node->child->brother->type, aux2);
    printf("%%%d = srem i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    /*
    fairly sure que nao ha divisao inteira de doubles em c
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->type, aux2);
      printf("%%%d = frem double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    */
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Not: 
    generate_code(node->child);
    // Vou assumir que NUNCA ha !Double
    aux1 = convert_register(Int, node->child->type, r_count - 1);
    printf("%%%d = icmp eq i32 %%%d, 0\n", r_count++, aux1);
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Minus:
    generate_code(node->child);

    if (node->type != Double){
      aux1 = r_count - 1;
      printf("%%%d = sub nsw %s %s, %%%d\n", r_count++, get_llvm_type(node->type), get_default_value(node->type), aux1);
    }
    else{
      aux1 = convert_register(Double, node->child->type, r_count - 1);
      printf("%%%d = fsub double -%s, %%%d\n", r_count++, get_default_value(Double), aux1);
    }
    
    if (node->brother != NULL)
      generate_code(node->brother);
    break;

  case Plus:
    full_generation(node);
    break;

  case RealLit:
    printf("%%%d = fadd double %s, %s\n", r_count++, get_default_value(Double), handle_constant(Double, node->value));
    break;
  case IntLit:
    printf("%%%d = add i32 %s, %s\n", r_count++, get_default_value(Int), handle_constant(Int, node->value));
    break;
  case ChrLit:
    printf("%%%d = add i32 %s, %s\n", r_count++, get_default_value(Char), handle_constant(Char, node->value));
    // isto esta provavelmente mal. Devo ter que fazer i8 se possivel ou conversao para i32 beforehand
    break;
  case Id:
    printf("%%%d = load %s, %s* %%%s\n", r_count++, get_llvm_type(node->type), get_llvm_type(node->type), node->value);
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

int convert_register(Label target, Label original_l, int original_r){
  if (target == original_l)
    return original_r;  // no conversion needed

  switch (target)
  {
  case Char:
    if (original_l == Double){
      printf("%%%d = fptosi double %%%d to i8\n", r_count++, original_r);
      break;
    }
    printf("%%%d = trunc %s %%%d to i8\n", r_count++, get_llvm_type(original_l), original_r);
    break;

  case Short:
    if (original_l == Double){
      printf("%%%d = fptosi double %%%d to i16\n", r_count++, original_r);
      break;
    }
    if (original_l == Char){
      printf("%%%d = sext i8 %%%d to i16\n", r_count++, original_r);
      break;
    }
    printf("%%%d = trunc %s %%%d to i16\n", r_count++, get_llvm_type(original_l), original_r);
    break;

  case Int:
    if (original_l == Double){
      printf("%%%d = fptosi double %%%d to i32\n", r_count++, original_r);
      break;
    }
    printf("%%%d = sext %s %%%d to i32\n", r_count++, get_llvm_type(original_l), original_r);
    break;

  case Double:
    printf("%%%d = sitofp %s %%%d to double\n", r_count++, get_llvm_type(original_l), original_r);
    break;

  default:
    printf("FATAL: Invalid conversion type %s\n", get_label_string(target));
  }
  return r_count - 1;
}

char *get_llvm_type(Label label)
{
  char *s = NULL;
  switch (label)
  {
  /*TODO: estes dois sao gerados com signext antes do tamanho, verificar se e necessario*/
  case Char:
    s = "i8";   //se apanho o tolo que pos isto a i32
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