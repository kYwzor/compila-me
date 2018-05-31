#include "generation.h"

// TODO: REMEMBER TO ALSO CHECK FOR INT CONVERSIONS ON OPERATIONS
// IT'S POSSIBLE THAT WE HAVE AN ID
// ALSO CHECK IF TWO CHAR IDS CAN MAKE A CHAR ADD

int r_count = 1;
Label current_function_type = -1;
Register_list register_list;
void generate_code(Node node)
{
  int aux1, aux2;
  char aux_str[1024];
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
    register_list = (Register_list)malloc(sizeof(struct _rl));
    register_list->next = NULL;
    register_list->id = (char *)malloc(sizeof(char) * 1024);
    register_list->updated_register = (char *)malloc(sizeof(char) * 1024);
    register_list->id = "Hi i'm list head";
    register_list->updated_register = "Hi i'm list head";
    Node aux = node->child;
    while (aux != NULL)
    {
      generate_code(aux);
      aux = aux->brother;
    }

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

    Node aux = paramList->child;
    char param_string[1024];
    char aux_string[1024];
    strcpy(param_string, "");
    while (aux != NULL && aux->child->label != Void)
    {
      aux_string[0] = '\0';
      if (param_string[0] != '\0')
        sprintf(aux_string, ", %s %%%s", get_llvm_type(aux->child->label), aux->child->brother->value);
      else
        sprintf(aux_string, "%s %%%s", get_llvm_type(aux->child->label), aux->child->brother->value);
      strcat(param_string, aux_string);
      aux = aux->brother;
    }
    printf("define %s @%s(%s){\n", get_llvm_type(type_spec->label), id->value, param_string);
    
    aux = paramList->child;
    generate_code(aux);

    //printf("ret %s %s\n}\n", get_llvm_type(type_spec->label), get_default_value(type_spec->label)); //return default, fica no final da funcao, provavelmente inalcancavel. Isto e suposto ser assim
    aux = paramList->brother->child; //funcbody child
    while (aux != NULL)
    {
      generate_code(aux);
      aux = aux->brother;
    }
    current_function_type = -1;

    printf("\tret %s %s\n}\n", get_llvm_type(type_spec->label), get_default_value(type_spec->label)); //return default, fica no final da funcao, provavelmente inalcancavel. Isto e suposto ser assim
    break;
  }
    /*
  case FuncDeclaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDeclaration));
    //full_generation(node);
    break;
  }
*/
  case ParamDeclaration:
  {
    Node typeSpec = node->child;
    Node id = typeSpec->brother;
    if (typeSpec->label != Void)
    {
      printf("%%%s = alloca %s\n", id->value, get_llvm_type(typeSpec->label));
      printf("store %s %%%s, %s* %%%d\n", get_llvm_type(typeSpec->label), id->value, get_llvm_type(typeSpec->label), r_count - 1);
    }
    if (node->brother != NULL)
      generate_code(node->brother);
    break;
  }
  case Declaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Declaration));
    Node type_spec = node->child;
    Node id = type_spec->brother;
    Node aux = id->brother;
    char *register_id = (char *)malloc(sizeof(char) * 1024);
    if (current_function_type != -1)
    {
      sprintf(register_id, "%%%s", id->value);
      insert_alias(id->value, register_id, type_spec->label);
      printf("%s = alloca %s\n", get_register(id->value)->updated_register, get_llvm_type(type_spec->label)); //align???
      if (aux != NULL)
      {
        generate_code(aux);
        aux1 = convert_register(type_spec->label, aux->type, r_count - 1);
        printf("\tstore %s %%%d, %s* %%%s\n", get_llvm_type(type_spec->label), aux1, get_llvm_type(type_spec->label), id->value);
      }
    }
    else
    {
      sprintf(register_id, "@%s", id->value);
      insert_alias(id->value, register_id, type_spec->label);
      if (aux != NULL)
      {
        if (type_spec->label != Double)
        {
          printf("%s = global %s %d\n", get_register(id->value)->updated_register, get_llvm_type(type_spec->label), eval_int(aux));
        }
        else
        {
          printf("%s = global %s %lf\n", get_register(id->value)->updated_register, get_llvm_type(type_spec->label), /*meter aqui uma versao double*/ eval_int(aux));
        }
      }
      else
      {
        printf("%s = common global %s %s\n", get_register(id->value)->updated_register, get_llvm_type(type_spec->label), get_default_value(type_spec->label));
      }
    }

    break;
  }

  case Return:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Return));
    if (node->child == NULL)
    {
      printf("\tret void\n");
      break;
    }
    generate_code(node->child);
    aux1 = convert_register(current_function_type, node->child->type, r_count - 1);
    printf("\tret %s %%%d\n", get_llvm_type(current_function_type), aux1);
    break;
  }

  case Store:
    generate_code(node->child->brother);
    aux1 = convert_register(node->type, node->child->brother->type, r_count - 1);
    printf("\tstore %s %%%d, %s* %%%s\n", get_llvm_type(node->type), aux1, get_llvm_type(node->type), node->child->value);
    break;

  /* :(
  case Or:
  case And:
  */
  case Eq:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp eq i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp oeq double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;

  case Ne:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp ne i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp une double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;
  case Lt:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp slt i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp olt double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;
  case Gt:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp sgt i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp ogt double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;
  case Le:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp sle i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp ole double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;
  case Ge:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if ((node->child->type != Double) && (node->child->brother->type != Double))
    {
      aux1 = convert_register(Int, node->child->type, aux1);
      aux2 = convert_register(Int, node->child->brother->type, aux2);
      printf("\t%%%d = icmp sge i32 %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fcmp oge double %%%d, %%%d\n", r_count++, aux1, aux2);
      aux1 = r_count - 1;
      printf("\t%%%d = zext i1 %%%d to i32\n", r_count++, aux1);
    }
    break;

  case Add:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type != Double)
    {
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("\t%%%d = add %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fadd double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    break;

  case Sub:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type != Double)
    {
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("\t%%%d = sub %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fsub double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    break;

  case Mul:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type != Double)
    {
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("\t%%%d = mul %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fmul double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    break;

  case Div:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;

    if (node->type != Double)
    {
      aux1 = convert_register(node->type, node->child->type, aux1);
      aux2 = convert_register(node->type, node->child->brother->type, aux2);
      printf("\t%%%d = sdiv %s %%%d, %%%d\n", r_count++, get_llvm_type(node->type), aux1, aux2);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->brother->type, aux2);
      printf("\t%%%d = fdiv double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    break;

  case Mod:
    generate_code(node->child);
    aux1 = r_count - 1;
    generate_code(node->child->brother);
    aux2 = r_count - 1;
    aux1 = convert_register(Int, node->child->type, aux1);
    aux2 = convert_register(Int, node->child->brother->type, aux2);
    printf("\t%%%d = srem i32 %%%d, %%%d\n", r_count++, aux1, aux2);
    /*
    fairly sure que nao ha divisao inteira de doubles em c
    else{
      aux1 = convert_register(Double, node->child->type, aux1);
      aux2 = convert_register(Double, node->child->type, aux2);
      printf("\t%%%d = frem double %%%d, %%%d\n", r_count++, aux1, aux2);
    }
    */
    break;

  case Not:
    generate_code(node->child);
    // Vou assumir que NUNCA ha !Double
    aux1 = convert_register(Int, node->child->type, r_count - 1);
    printf("\t%%%d = icmp eq i32 %%%d, 0\n", r_count++, aux1);
    break;

  case Minus:
    generate_code(node->child);

    if (node->type != Double)
    {
      aux1 = r_count - 1;
      printf("\t%%%d = sub nsw %s %s, %%%d\n", r_count++, get_llvm_type(node->type), get_default_value(node->type), aux1);
    }
    else
    {
      aux1 = convert_register(Double, node->child->type, r_count - 1);
      printf("\t%%%d = fsub double -%s, %%%d\n", r_count++, get_default_value(Double), aux1);
    }
    break;

  case Plus:
    generate_code(node->child);
    break;

  case RealLit:
    printf("\t%%%d = fadd double %s, %s\n", r_count++, get_default_value(Double), handle_constant(Double, node->value, aux_str));
    break;
  case IntLit:
    printf("\t%%%d = add i32 %s, %s\n", r_count++, get_default_value(Int), handle_constant(Int, node->value, aux_str));
    break;
  case ChrLit:
    printf("\t%%%d = add i32 %s, %s\n", r_count++, get_default_value(Char), handle_constant(Char, node->value, aux_str));
    // sim, tem mesmo que ser i32, porque um chrlit e sempre anotado como int
    break;
  case Id:
  {
    Register_list aux = get_register(node->value);
    printf("\t%%%d = load %s, %s* %s\n", r_count++, get_llvm_type(aux->type), get_llvm_type(aux->type), aux->updated_register);
    convert_register(node->type, aux->type, r_count - 1);
    break;
  }
  case Call:
  {
    Node aux = node->child->brother;
    char *param_string = (char *)malloc(sizeof(char) * 1024);
    char aux_string[1024];
    strcpy(param_string, "");
    Arg_list arguments = get_function_args(node->child->value);
    while (aux != NULL)
    {
      generate_code(aux);
      aux_string[0] = '\0';
      if (param_string[0] != '\0')
        sprintf(aux_string, ", %s %%%d", get_llvm_type(arguments->label), convert_register(arguments->label, aux->type, r_count - 1));
      else
        sprintf(aux_string, "%s %%%d", get_llvm_type(arguments->label), convert_register(arguments->label, aux->type, r_count - 1));
      strcat(param_string, aux_string);
      aux = aux->brother;
      arguments = arguments->next;
    }
    if (node->type != Void)
    {

      printf("\t%%%d = call %s @%s(%s)\n", r_count++, get_llvm_type(node->type), node->child->value, param_string);
    }
    else
    {
      printf("\tcall %s @%s(%s)\n", get_llvm_type(node->type), node->child->value, param_string);
    }
    break;
  }
  default:
    if (DEBUG)
      printf("Defaulted %s\n", get_label_string(node->label));
    //full_generation(node);
    if (node->child != NULL)
      generate_code(node->child);
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

int eval_int(Node node)
{
  switch (node->label)
  {
  case Or:
    return eval_int(node->child) || eval_int(node->child->brother);
  case And:
    return eval_int(node->child) && eval_int(node->child->brother);
  case Eq:
    return eval_int(node->child) == eval_int(node->child->brother);
  case Ne:
    return eval_int(node->child) != eval_int(node->child->brother);
  case Lt:
    return eval_int(node->child) < eval_int(node->child->brother);
  case Le:
    return eval_int(node->child) <= eval_int(node->child->brother);
  case Gt:
    return eval_int(node->child) > eval_int(node->child->brother);
  case Ge:
    return eval_int(node->child) >= eval_int(node->child->brother);
  case Mod:
    return eval_int(node->child) % eval_int(node->child->brother);
  case Comma:
    //Pensar amanha depois de beber um cafe
    break;
  case Add:
    return eval_int(node->child) + eval_int(node->child->brother);
  case Sub:
    return eval_int(node->child) - eval_int(node->child->brother);
  case Mul:
    return eval_int(node->child) * eval_int(node->child->brother);
  case Div:
    return eval_int(node->child) / eval_int(node->child->brother);
  case BitWiseAnd:
    return eval_int(node->child) & eval_int(node->child->brother);
  case BitWiseXor:
    return eval_int(node->child) ^ eval_int(node->child->brother);
  case BitWiseOr:
    return eval_int(node->child) | eval_int(node->child->brother);
  case Store:
    //Pensar amanha depois de beber um cafe
    break;
  case Not:
    return !eval_int(node->child);
  case Minus:
    return -eval_int(node->child);
  case Plus:
    return +eval_int(node->child);
  case IntLit:
  {
    int aux_int;
    if (node->value[0] == '0')
    {
      sscanf(node->value, "%o", &aux_int);
    }
    else
    {
      sscanf(node->value, "%d", &aux_int);
    }
    return aux_int;
    break;
  }
  case ChrLit:
  {
    char aux_char = '\0';
    //printf("value %s\n", value);
    if (node->value[3] != '\0')
    {
      if (node->value[2] == 'n')
      {
        aux_char = '\n';
      }
      else if (node->value[2] == 't')
      {
        aux_char = '\t';
      }
      else if (node->value[2] == '\\')
      {
        aux_char = '\\';
      }
      else if (node->value[2] == '\'')
      {
        aux_char = '\'';
      }
      else if (node->value[2] == '"')
      {
        aux_char = '"';
      }
    }
    else{
      aux_char = node->value[1];
    }
    return aux_char;
  }
  default:
    printf("Fatal error in eval_int %s\n", get_label_string(node->label));
    return ERROR;
  }
  return ERROR;
}
char * convert_register_id(Label target, Label origin_rl, char* id)
{
  if (target == origin_rl)
    return id; // no conversion needed

  char * register_id = (char*)malloc(sizeof(char) * 1024);
  switch (target)
  {
  case Char:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%s to i8\n", r_count++, id);
      break;
    }
    printf("\t%%%d = trunc %s %%%s to i8\n", r_count++, get_llvm_type(origin_rl), id);
    break;

  case Short:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%s to i16\n", r_count++, id);
      break;
    }
    if (origin_rl == Char)
    {
      printf("\t%%%d = sext i8 %%%s to i16\n", r_count++, id);
      break;
    }
    printf("\t%%%d = trunc %s %%%s to i16\n", r_count++, get_llvm_type(origin_rl), id);
    break;

  case Int:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%s to i32\n", r_count++, id);
      break;
    }
    printf("\t%%%d = sext %s %%%s to i32\n", r_count++, get_llvm_type(origin_rl), id);
    break;

  case Double:
    printf("\t%%%d = sitofp %s %%%s to double\n", r_count++, get_llvm_type(origin_rl), id);
    break;

  default:
    printf("FATAL: Invalid conversion type %s\n", get_label_string(target));
  }
  sprintf(register_id, "%d", r_count - 1);
  return register_id;
}
int convert_register(Label target, Label origin_rl, int original_r)
{
  if (target == origin_rl)
    return original_r; // no conversion needed

  switch (target)
  {
  case Char:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%d to i8\n", r_count++, original_r);
      break;
    }
    printf("\t%%%d = trunc %s %%%d to i8\n", r_count++, get_llvm_type(origin_rl), original_r);
    break;

  case Short:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%d to i16\n", r_count++, original_r);
      break;
    }
    if (origin_rl == Char)
    {
      printf("\t%%%d = sext i8 %%%d to i16\n", r_count++, original_r);
      break;
    }
    printf("\t%%%d = trunc %s %%%d to i16\n", r_count++, get_llvm_type(origin_rl), original_r);
    break;

  case Int:
    if (origin_rl == Double)
    {
      printf("\t%%%d = fptosi double %%%d to i32\n", r_count++, original_r);
      break;
    }
    printf("\t%%%d = sext %s %%%d to i32\n", r_count++, get_llvm_type(origin_rl), original_r);
    break;

  case Double:
    printf("\t%%%d = sitofp %s %%%d to double\n", r_count++, get_llvm_type(origin_rl), original_r);
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

char *handle_constant(Label type, char *value, char *aux_str)
{
  switch (type)
  {
  case Double:
  {
    double aux_double;
    //printf("value %s\n", value);
    sscanf(value, "%lf", &aux_double);
    //printf("aux_double %lf\n", aux_double);
    sprintf(aux_str, "%.16E", aux_double); //verificar quantas casas devem ser
    //printf("aux_str %s\n", aux_str);
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
    aux_str = value;
    break;
  case Int:
    if (value[0] == '0')
    {
      int aux_int;
      sscanf(value, "%o", &aux_int);
      sprintf(aux_str, "%d", aux_int);
    }
    else
    {
      aux_str = value;
    }
    break;
  case Char:
  {
    char aux_char = '\0';
    //TODO: Temos de fazer um caso especial para os caracteres \t \n e assim
    //printf("value %s\n", value);
    if (value[3] != '\0')
    {
      if (value[2] == 'n')
      {
        aux_char = '\n';
      }
      else if (value[2] == 't')
      {
        aux_char = '\t';
      }
      else if (value[2] == '\\')
      {
        aux_char = '\\';
      }
      else if (value[2] == '\'')
      {
        aux_char = '\'';
      }
      else if (value[2] == '"')
      {
        aux_char = '"';
      }
    }
    else
    {
      sscanf(value, "'%c'", &aux_char); //thought this would work :(
    }
    //printf("aux_char %c\n", aux_char);
    sprintf(aux_str, "%d", aux_char);
    //printf("aux_str %s\n", aux_str);
    break;
  }
  case Void:
    sprintf(aux_str, "");
    break;
  default:
    printf("FATAL: Invalid constant type\n");
  }
  return aux_str;
}

void insert_alias(char *id, char *updated_register, Label type)
{
  Register_list aux = register_list;
  while (aux != NULL)
  {
    if (strcmp(aux->id, id) == 0)
    {
      if (DEBUG)
        printf("Updated %s register from %s to %s\n", id, aux->updated_register, updated_register);
      aux->updated_register = updated_register;
      aux->type = type;
      return;
    }
    aux = aux->next;
  }
  Register_list alias = (Register_list)malloc(sizeof(struct _rl));
  alias->id = id;
  alias->updated_register = updated_register;
  alias->next = register_list;
  alias->type = type;
  register_list = alias;
}

Register_list get_register(char *id)
{
  Register_list aux = register_list;
  while (aux != NULL)
  {
    if (strcmp(aux->id, id) == 0)
    {
      return aux;
    }
    aux = aux->next;
  }
  printf("This should never happen, could not find register\n");
  return NULL;
}