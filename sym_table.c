#include "tree.h"
typedef enum {INT, CHAR, SHORT, DOUBLE, VOID} basic_type;

typedef struct _t1{
	char name[1024];
	basic_type type;
	struct _t1 *next;
}
void insert_symbol(){

}
