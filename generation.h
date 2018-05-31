#include "semantics.h"

void full_generation(Node node);
void generate_code(Node node);
char *handle_constant(Label type, char *value);
char* get_llvm_type(Label label);
int convert_register(Label target, Label original_l, int original_r);
char *get_default_value(Label label);
Register_list get_register(char* id);
void insert_alias(char* id, char* updated_register,Label llvm_type);
int eval_int(Node node);
void clean_up_register();
char * convert_register_id(Label target, Label origin_rl, char* id);