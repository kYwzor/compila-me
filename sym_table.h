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
int insert_symbol();

Table_list list;
Sym_table current_table;
