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
	if(errorFlag!=1 && flag > 1){
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
			/*
			Table_list aux = find_function_entry(current->value);
			if(aux != NULL){
				//Verificar se existe variavel local com o mesmo nome
				Sym_list local_check = find_symbol(aux, current->value);
				Arg_list arg_check = find_parameter(aux, current->value);
				if(local_check == NULL && DEBUG) printf("#%s is not a local variable\n", current->value);
				if(arg_check == NULL && DEBUG) printf("#%s is not an argument \n", current->value);
				if(strcmp(current->value, aux->table_node->name) == 0 && DEBUG) printf("#%s is the name of the function, overriding\n", current->value);
				//Sym_list local_check = NULL;
				if((local_check == NULL && arg_check == NULL) || strcmp(current->value, aux->table_node->name) == 0){
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
			*/
		}
		printf("\n");
	}

	if(current->child != NULL) print_tree(current->child, depth+1);
	if(current->brother != NULL) print_tree(current->brother, depth);
}


void free_tree(Node current){
	if(current->child != NULL) free_tree(current->child);
	if(current->brother != NULL) free_tree(current->brother);

	free(current->value);
	free(current);
}
