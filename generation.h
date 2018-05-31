#include "semantics.h"

void full_generation(Node node);
void generate_code(Node node);
char *handle_constant(Label type, char *value, char *aux_str);
char* get_llvm_type(Label label);
int convert_register(Label target, Label original_l, int original_r);
char *get_default_value(Label label);
int eval_int(Node node);
char* get_register(char* value);
double eval_double(Node node);