#include "structs.h"


Node create_node(Label label, char *value);
void add_child(Node father, Node child);
void add_brother(Node older, Node newer);
void print_tree_and_free(Node current, int depth);
char* get_label_string(Label label);

Node treeRoot;
Node aux;
Node aux2;
int errorFlag;
int flag;
