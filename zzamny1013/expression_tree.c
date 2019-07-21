#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree( void);

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
TREE *destroyTree( TREE *pTree);

static void _destroy( NODE *root);

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode( char ch);

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree);

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr);

int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];

	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}


TREE *createTree( void){
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	newTree -> root = NULL;

	return newTree;
}

TREE *destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
	
	return NULL;	
}

static void _destroy( NODE *root)
{
	if(root != NULL){
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}
}

static NODE *_makeNode( char ch){
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if(newNode == NULL)
		return NULL;
	
	newNode->data = ch;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

int postfix2tree( char *expr, TREE *pTree){
	NODE* stack[MAX_STACK_SIZE];
	int index = -1;
	for(int i=0; expr[i] != '\0'; i++){
		NODE* newNode = _makeNode(expr[i]);
		if(newNode->data >= 48 && newNode->data <= 57){
			index++;
			stack[index] = newNode;
		}
		else if(newNode->data == 42 || newNode->data == 43 || newNode->data == 45 || newNode->data == 47){
			if(index < 1){
				_destroy(newNode);
				_destroy(stack[0]);
				destroyTree(pTree);
				return 0;
			}
			newNode->left = stack[index-1];
			newNode->right = stack[index];
			stack[index-1] = newNode;
			stack[index] = NULL;
			index--;
		}
		else{
			_destroy(newNode);
			for(int i=0; i<=index; i++){
				_destroy(stack[i]);
			}
			destroyTree(pTree);
			return 0;
		}
	}
	if(stack[1] != NULL){
		for(int i=0; i<=index; i++){
			_destroy(stack[i]);
		}
		destroyTree(pTree);
		return 0;
	}
	pTree->root = stack[0];

	return 1;

}

void traverseTree( TREE *pTree)
{
	_traverse (pTree->root);
	
	return;
}

static void _traverse( NODE *root)
{
	if(root != NULL){
		if(root->data >= 48 && root->data <=57){
			printf("%c", root->data);
		}
		else{
			printf("(");
			_traverse(root->left);
			printf("%c", root->data);
			_traverse(root->right);
			printf(")");
		}
	}
}

void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

static void _infix_print( NODE *root, int level)
{
	if(root != NULL){
		level++;
		_infix_print(root->right, level);
		for(int i=1; i<level; i++){
			printf("\t");
		}
		printf("%c\n", root->data);
		
		_infix_print(root->left, level);
	}
}

float evalPostfix( char *expr)
{
	float stack[MAX_STACK_SIZE];
	int topIndex = -1;
	int value;
	
	for(int i=0; expr[i] != '\0'; i++){
		if(expr[i] >= 48 && expr[i] <= 57){
			topIndex++;
			stack[topIndex] = (float)(expr[i]-48);
		}
		else if(expr[i] == 43){
			stack[topIndex-1] = stack[topIndex-1] + stack[topIndex];
			topIndex--;
		}
		else if(expr[i] == 45){
			stack[topIndex-1] = stack[topIndex-1] - stack[topIndex];
			topIndex--;
		}
		else if(expr[i] == 42){
			stack[topIndex-1] = stack[topIndex-1] * stack[topIndex];
			topIndex--;
		}
		else if(expr[i] == 47){
			stack[topIndex-1] = stack[topIndex-1] / stack[topIndex];
			topIndex--;
		}
	}

	return stack[0];
}
