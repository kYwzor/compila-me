#include "sym_list.h"

char* get_string_for_tables(Label label);
int handle_node(Node node); 
void semantic_analysis(Node node);
void print_tables();
void full_expand(Node node);
Table_list find_function_entry(char* name);
Table_list create_function_entry(char* name, Label label, Node paramList, int is_definition);
void put_type(Node node);
Label get_type(Node node);
Label resolve_type(Label label1, Label label2);

