#include "tree.h"
Node create_node(Label label, char *value){
	Node newNode = malloc(sizeof(Node_t));

	newNode->label = label;
	newNode->type = Empty;
	newNode->arg_list = NULL;

	newNode->value = value; //value is either NULL or comes from yylval.value which was strduped
	
	newNode->child = NULL;
	newNode->brother = NULL;

	return newNode;
}

void add_child(Node father, Node child){
	father->child = child;
}

Node add_brother(Node older, Node newer){
	if(older==NULL){
		return newer;
	}
	if(newer==NULL){
		return older;
	}
	Node aux = older;
	while(aux->brother!=NULL)
		aux = aux->brother;

	aux->brother = newer;
	return older;
}

void print_tree_and_free(Node current, int depth){
	int i;
	if(errorFlag!=1 && flag > 1){
		for(i=0; i<depth; i++)
			printf("..");
		printf("%s", get_label_string(current->label));
		if(current->value!=NULL){
			printf("(%s)", current->value);
		}
		if(current->type != Empty){
			printf(" - %s",get_string_for_tables(current->type));
			Table_list aux = find_function_entry(current->value);
			if(aux != NULL){
				printf("(");
				//This id is for a function
				Arg_list arg_list = aux->arg_list;
				while(arg_list != NULL){
					printf("%s", get_string_for_tables(arg_list->label));
					arg_list = arg_list->next;
					if(arg_list != NULL)
						printf(",");
				}
				printf(")");
			}
		}
		printf("\n");

	}

	if(current->child != NULL) print_tree_and_free(current->child, depth+1);
	if(current->brother != NULL) print_tree_and_free(current->brother, depth);

	free(current->value);
	free(current);
}


