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

void print_tree(Node current, int depth){
	int i;
	for(i=0; i<depth; i++)
		printf("..");
	printf("%s", get_label_string(current->label));
	if(current->value!=NULL){
		printf("(%s)", current->value);
	}
	if(current->type != Empty){
		printf(" - %s",get_string_for_tables(current->type));
		if(current->arg_list != NULL){
			Arg_list aux_arg = current->arg_list;
			printf("(");
			while(aux_arg != NULL){
				printf("%s", get_string_for_tables(aux_arg->label));
				aux_arg= aux_arg->next;
				if(aux_arg != NULL)
					printf(",");
			}
			printf(")");
		}	
	}
	printf("\n");

	if(current->child != NULL) print_tree(current->child, depth+1);
	if(current->brother != NULL) print_tree(current->brother, depth);
}


void free_tree(Node current){
	if(current->child != NULL) free_tree(current->child);
	if(current->brother != NULL) free_tree(current->brother);

	free(current->value);
	free(current);
}
