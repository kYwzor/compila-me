#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s *Node;
typedef struct node_s{
	char *label;
	char *value;
	Node child;
	Node brother;
}Node_t;

Node createNode(char *label, char *value);
void addChild(Node father, Node child);
void addBrother(Node older, Node newer);
void printTreeAndFree(Node current, int depth);

Node treeRoot;
Node aux;
Node aux2;
int errorFlag;