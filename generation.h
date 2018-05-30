#include "structs.h"

void full_generation(Node node);
char *get_label_string(Label label);
void generate_code(Node node);
char *handle_constant(Label type, char *value);
char* get_llvm_type(Label label);
char *get_default_value(Label label);