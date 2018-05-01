#include "tree.h"
Node create_node(Label label, char *value){
	Node newNode = malloc(sizeof(Node_t));

	newNode->label = label;
	newNode->type = NULL;

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
		if(current->value!=NULL) printf("%s(%s)\n", get_label_string(current->label), current->value);
		else printf("%s\n", get_label_string(current->label));
	}

	if(current->child != NULL) print_tree_and_free(current->child, depth+1);
	if(current->brother != NULL) print_tree_and_free(current->brother, depth);

	free(current->value);
	free(current);
}


