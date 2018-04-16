#include "tree.h"
#include "sym_table.h"

Symbol_table global_table;

void semantic_analysis(Node node){
   handle_node(node);
   if(node->child != NULL)
    semantic_analysis(node);
}

void handle_node(Node node){
    switch(node->label){
        case Program:
		            
    }

}
