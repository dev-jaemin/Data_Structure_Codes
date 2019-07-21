#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
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
TREE *BST_Create( void);

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
TREE *BST_Destroy( TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

static NODE *_makeNode( int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree);
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);


////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	int data;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 1; i < 20; i++)
	{
		data = rand() % 100 + 1; // 1 ~ 100 random number
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		BST_Insert( tree, data);
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	BST_Traverse(tree);

	int ret;
	do
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret) fprintf( stdout, "%d not found\n", num);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		BST_Traverse(tree);
		
	} while(1);
	
	BST_Destroy( tree);

	return 0;
}

TREE *BST_Create( void)
{
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	if(newTree == NULL)
		return NULL;
	
	newTree->root = NULL;
	return newTree;
}

TREE *BST_Destroy( TREE *pTree)
{
	_destroy(pTree->root);
	free(pTree);

	return pTree;
}
static void _destroy( NODE *root)
{
	if(root != NULL){
		_destroy(root->left);
		_destroy(root->right);
		free(root);
		root = NULL;
	}
}

int BST_Insert( TREE *pTree, int data)
{
	NODE* newNode = _makeNode(data);
	if(newNode == NULL){
		return 0;
	}
	if(pTree->root == NULL){
		pTree->root = newNode;
		return 1;
	}
	NODE* curNode = pTree->root;
	while(1){
		if(curNode->data > data){
			if(curNode->left == NULL){
				curNode->left = newNode;
				return 1;
			}
			curNode = curNode->left;
		}
		else{
			if(curNode->right == NULL){
				curNode->right = newNode;
				return 1;
			}
			curNode = curNode->right;
		}

	}
	
}

static NODE *_makeNode( int data)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

int BST_Delete( TREE *pTree, int dltKey)
{
	int success;
	pTree->root = _delete(pTree->root, dltKey, &success);

	return success;
}

NODE *_delete( NODE *root, int dltKey, int *success)
{
	NODE* pVRoot = _makeNode(0);
	NODE* pNode = pVRoot;
	NODE* curNode = root;
	NODE* delNode;
	pVRoot->right = root;


	while(curNode != NULL && curNode->data != dltKey){
		pNode = curNode;
		if(dltKey < curNode->data){
			curNode = curNode->left;
		}
		else{
			curNode = curNode->right;
		}
	}

	if(curNode == NULL){
		*success = 0;
		free(pVRoot);
		return root;
	}

	delNode = curNode;

	if(delNode->left == NULL && delNode->right == NULL){
		if(pNode->left == delNode){
			free(pNode->left);
			pNode->left = NULL;
		}
		else{
			if(delNode == root){
				free(root);
				root = NULL;
				pVRoot->right = NULL;
			}
			else{
				free(pNode->right);	
				pNode->right = NULL;
			}
		}
	}
	else if(delNode->left == NULL || delNode->right == NULL){
		NODE* dcNode;
		if(delNode->left != NULL){
			dcNode = delNode->left;
		}
		else{
			dcNode = delNode->right;
		}
		if(pNode->left == delNode){
			pNode->left = dcNode;
			free(delNode);
			delNode = NULL;
		}
		else{
			if(delNode == root){
				pVRoot->right = dcNode;
				root = dcNode;
			}
			pNode->right = dcNode;
			free(delNode);
			delNode = NULL;
		}
	}
	else{
		NODE* mNode = delNode->right;
		NODE* mpNode = delNode;
		if(mNode->left == NULL){
			delNode->data = mNode->data;
			mNode = mNode->right;
			free(mpNode->right);
			mpNode->right = mNode;
		}
		else{
			while(mNode->left != NULL){
				mpNode = mNode;
				mNode = mNode->left;
			}
			delNode->data = mNode->data;
			mNode = mNode->right;
			free(mpNode->left);
			mpNode->left = NULL;
			mpNode->left = mNode;
		}
	}

	
	if(pVRoot->right != root){
		root = pVRoot->right;
	}
	*success = 1;
	free(pVRoot);
	pVRoot = NULL;

	return root;
}

/*int *BST_Retrieve( TREE *pTree, int key)
{
	NODE* retNode = _retrieve(pTree->root, key);
	
	return retNode;
}*/

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
/*NODE *_retrieve( NODE *root, int key)
{
	if(root->data == key){
		return root;
	}
	else if(root->data > key){
		return _retrieve(root->right, key);
	}
	else{
		return _retrieve(root->left, key);
	}

	return NULL;
}*/

void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
	printf("\n");
}
static void _traverse( NODE *root)
{
	if(root != NULL){
		_traverse(root->left);
		printf("%d ", root->data);
		_traverse(root->right);
	}
}

void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
}

static void _infix_print( NODE *root, int level)
{
	if(root != NULL){
		level++;
		_infix_print(root->right, level);
		for(int i=1; i<level; i++){
			printf("\t");
		}
		printf("%d\n", root->data);
		
		_infix_print(root->left, level);
	}
}
