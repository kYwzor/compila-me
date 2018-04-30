#include "sym_list.h"

char* get_string_for_tables(Label label);
int handle_node(Node node); 
void semantic_analysis(Node node);
void add_table(Sym_list table, int is_defined); 
void print_tables();
char* get_label_string(Label label);
void add_parameter(Node type_spec, Node id);
void full_expand(Node node);
Arg_list get_function_args(char * name);
Sym_list find_function_entry(Table_list list, Label label, char *value);


Table_list list;
Sym_list current_table;
Sym_list global_table;

