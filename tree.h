#include "semantics.h" 

Node create_node(Label label, char *value);
void add_child(Node father, Node child);
Node add_brother(Node older, Node newer);
void print_tree(Node current, int depth);
void free_tree(Node current);

Node treeRoot;
Node aux;
Node aux2;
int errorFlag;
int flag;
