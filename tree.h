#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum _label{
	Program, Declaration, FuncDeclaration, FuncDefinition, ParamList, FuncBody, ParamDeclaration, StatList, If, While, Return, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Not, Minus, Plus, Store, Comma, Call, BitWiseAnd, BitWiseXor, BitWiseOr, Char, ChrLit, Id, Int, Short, IntLit, Double, RealLit, Void, Null
}Label;
typedef enum _type {INT, CHAR, SHORT, DOUBLE, VOID} Basic_type;

typedef struct node_s *Node;
typedef struct node_s{
	Label label;
	Basic_type type;
	char *value;
	Node child;
	Node brother;
}Node_t;

Node createNode(Label label, char *value);
void addChild(Node father, Node child);
void addBrother(Node older, Node newer);
void printTreeAndFree(Node current, int depth);
void printNode(Node current);

Node treeRoot;
Node aux;
Node aux2;
int errorFlag;
int flag;
