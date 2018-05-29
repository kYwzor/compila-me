#include "structs.h"

void generate_code(Node node){
      if (DEBUG)
    printf("\tHandling %s %s\n", node->value, get_label_string(node->label));

  switch (node->label)
  {
  case Program:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Program));
    break;
  }

  case FuncDefinition:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDefinition));
    Node type_spec = node->child;
    Node id = type_spec->brother;
    Node paramList = id->brother;

    printf("define %s @%s(){\n", get_string_type(type_spec->label), id->value);
    handle_chil();


    printf("ret %s\n", );
    break;
  }

  case FuncDeclaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(FuncDeclaration));
    break;
  }

  case Declaration:
  {
    if (DEBUG)
      printf("%s is %s\n", get_label_string(node->label), get_label_string(Declaration));
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