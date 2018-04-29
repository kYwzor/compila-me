#include "semantics.h" 

Node create_node(Label label, char *value);
void add_child(Node father, Node child);
Node add_brother(Node older, Node newer);
void print_tree_and_free(Node current, int depth);

Node treeRoot;
Node aux;
Node aux2;
int errorFlag;
int flag;
