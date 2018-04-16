#include "tree.h"

Node createNode(Label label, char *value){
	Node newNode = malloc(sizeof(Node_t));

	newNode->label = label;

	newNode->value = value; //value is either NULL or comes from yylval.value which was strduped
	
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

	if(errorFlag!=1 && flag == 2){
		for(i=0; i<depth; i++)
			printf("..");
		printNode(current);
	}

	if(current->child != NULL) printTreeAndFree(current->child, depth+1);
	if(current->brother != NULL) printTreeAndFree(current->brother, depth);

	free(current->value);
	free(current);
}

void printNode(Node current){
	char* s;
	switch (current->label){
		case Program:
			s = "Program";
			break;
		case Declaration:
			s = "Declaration";
			break;
		case FuncDeclaration:
			s = "FuncDeclaration";
			break;
		case FuncDefinition:
			s = "FuncDefinition";
			break;
		case ParamList:
			s = "ParamList";
			break;
		case FuncBody:
			s = "FuncBody";
			break;
		case ParamDeclaration:
			s = "ParamDeclaration";
			break;
		case StatList:
			s = "StatList";
			break;
		case If:
			s = "If";
			break;
		case While:
			s = "While";
			break;
		case Return:
			s = "Return";
			break;
		case Or:
			s = "Or";
			break;
		case And:
			s = "And";
			break;
		case Eq:
			s = "Eq";
			break;
		case Ne:
			s = "Ne";
			break;
		case Lt:
			s = "Lt";
			break;
		case Gt:
			s = "Gt";
			break;
		case Le:
			s = "Le";
			break;
		case Ge:
			s = "Ge";
			break;
		case Add:
			s = "Add";
			break;
		case Sub:
			s = "Sub";
			break;
		case Mul:
			s = "Mul";
			break;
		case Div:
			s = "Div";
			break;
		case Mod:
			s = "Mod";
			break;
		case Not:
			s = "Not";
			break;
		case Minus:
			s = "Minus";
			break;
		case Plus:
			s = "Plus";
			break;
		case Store:
			s = "Store";
			break;
		case Comma:
			s = "Comma";
			break;
		case Call:
			s = "Call";
			break;
		case BitWiseAnd:
			s = "BitWiseAnd";
			break;
		case BitWiseXor:
			s = "BitWiseXor";
			break;
		case BitWiseOr:
			s = "BitWiseOr";
			break;
		case Char:
			s = "Char";
			break;
		case ChrLit:
			s = "ChrLit";
			break;
		case Id:
			s = "Id";
			break;
		case Int:
			s = "Int";
			break;
		case Short:
			s = "Short";
			break;
		case IntLit:
			s = "IntLit";
			break;
		case Double:
			s = "Double";
			break;
		case RealLit:
			s = "RealLit";
			break;
		case Void:
			s = "Void";
			break;
		case Null:
			s = "Null";
			break;
	}
	if(current->value!=NULL) printf("%s(%s)\n", s, current->value);
	else printf("%s\n", s);
}
