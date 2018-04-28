#include "sym_table.h"

int handle_node(Node node); 
void semantic_analysis(Node node);
void add_table(Sym_table table); 
void print_tables();
char* get_label_string(Label label);
void add_parameter(Node type_spec, Node id);

Table_list list;
Sym_table current_table;
Sym_table global_table;

