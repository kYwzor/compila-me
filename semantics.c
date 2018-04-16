#include "tree.h"
#include "sym_table.h"

//delet this
#define Program 0

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
