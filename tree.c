#include "tree.h"

Node createNode(char *label, char *value){
	Node newNode = malloc(sizeof(Node_t));

	newNode->label = strdup(label);

	if (value!= NULL) newNode->value = strdup(value);
	else newNode->value = NULL;
	
	newNode->child = NULL;
	newNode->brother = NULL;

	return newNode;
}

void addChild(Node father, Node child){
	father->child = child;
}

void addBrother(Node older, Node newer){
	if(older!=NULL && newer!=NULL){
		while(older->brother!=NULL)
			older = older->brother;

		older->brother = newer;
	}
}

void printTreeAndFree(Node current, int depth){
	int i;
	for(i=0; i<depth; i++)
		printf("..");

	if(current->value!=NULL) printf("%s(%s)\n", current->label, current->value);
	else printf("%s\n", current->label);

	if(current->child != NULL) printTreeAndFree(current->child, depth+1);
	if(current->brother != NULL) printTreeAndFree(current->brother, depth);

	free(current->label);
	free(current->value);
	free(current);
}