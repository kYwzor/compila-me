
typedef _sym_table * Sym_table;

typedef struct _t1{
	char* name;
	Label type;
	Sym_table next;
}_sym_table;

typedef _table_list *Table_list;

typedef struct _tl{
  Sym_table node;
  Table_list next;
  Arg_list arg_list;
}_table_list;

typedef _arg_list* Arg_list;

typedef struct _al{
  Label type;
  Arg_list next;
}_arg_list;

void insert_symbol();
Table_list list;
Sym_table current_table;
