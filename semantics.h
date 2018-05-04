#include "sym_list.h"

char* get_string_for_tables(Label label);
int handle_node(Node node); 
void semantic_analysis(Node node);
void print_tables();
char* get_label_string(Label label);
void full_expand(Node node);
Arg_list get_function_args(char * name);
Table_list find_function_entry(char* name);
Table_list create_function_entry(char* name, Label label, Node paramList, int is_definition);
void put_type(Node node);
Label get_type(Node node);
char * get_operator_string(Label label);
Label resolve_type(Label label1, Label label2);


Table_list global_table;
Table_list current_table;