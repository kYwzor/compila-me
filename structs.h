#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum _label{
	Program, Declaration, FuncDeclaration, FuncDefinition, ParamList, FuncBody, ParamDeclaration, StatList, If, While, Return, Or, And, Eq, Ne, Lt, Gt, Le, Ge, Add, Sub, Mul, Div, Mod, Not, Minus, Plus, Store, Comma, Call, BitWiseAnd, BitWiseXor, BitWiseOr, Char, ChrLit, Id, Int, Short, IntLit, Double, RealLit, Void, Null
}Label;

typedef struct node_s *Node;
typedef struct node_s{
	Label label;
	Label type;
	char *value;
	Node child;
	Node brother;
}Node_t;

typedef struct _t1* Sym_table;
typedef struct _t1{
  //TODO Mudar este nome para id
	char* name;
	Label type;
  Sym_table next;
}_sym_table;


typedef struct _tl* Table_list;
typedef struct _tl{
  Sym_table table_node;
  Table_list next;
  struct _tl* arg_list;
}_table_list;

typedef struct _al* Arg_list;
typedef struct _al{
  Label type;
  struct _al* next;
}_arg_list; 

