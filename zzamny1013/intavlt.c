#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time

#define MAX_ELEM 20
#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
AVL_TREE *AVL_Destroy( AVL_TREE *pTree);
static void _destroy( NODE *root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, int data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_makeNode( int data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *AVL_Retrieve( AVL_TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);

static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	int data;
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // random number
		// data = i+1; // sequential number
		
		fprintf( stdout, "%d ", data);

		// insert function call
		AVL_Insert( tree, data);
	}
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	AVL_Destroy( tree);

	return 0;
}


/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void)
{
	AVL_TREE* newTree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if(newTree == NULL){
		return NULL;
	}
	newTree->root = NULL;
	newTree->count = 0;

	return newTree;
}

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
AVL_TREE *AVL_Destroy( AVL_TREE *pTree)
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

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, int data)
{
	NODE* newNode = _makeNode(data);
	if(newNode == NULL){
		return 0;
	}
	
	pTree->root = _insert(pTree->root, newNode);
	(pTree->count)++;
	newNode->height = getHeight(newNode);
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr)
{
	if(root == NULL){
		root = newPtr;
		return root;
	}
	if(root->data > newPtr->data){
		root->left = _insert(root->left, newPtr);
		if((getHeight(root->left) - getHeight(root->right)) > 1){
			if(getHeight(root->left->left) > getHeight(root->left->right)){
				
				root = rotateRight(root);
			}
			else{
				root->left = rotateLeft(root->left);
				root = rotateRight(root);
			}
		}
	}
	else{
		root->right = _insert(root->right, newPtr);
		if((getHeight(root->right) - getHeight(root->left)) > 1){
			if(getHeight(root->right->right) > getHeight(root->right->left)){
				root = rotateLeft(root);
			}
			else{
				root->right = rotateRight(root->right);
				root = rotateLeft(root);
			}
		}
	}

	return root;
}

static NODE *_makeNode( int data)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = data;
	newNode->height = 0;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *AVL_Retrieve( AVL_TREE *pTree, int key)
{
	NODE* retNode = _retrieve(pTree->root, key);
	
	return (int*)retNode;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key)
{
	if(root->data == key){
		return root;
	}
	else if(root->data > key){
		_retrieve(root->right, key);
	}
	else{
		_retrieve(root->left, key);
	}

	return NULL;
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
	printf("\n");
}

static void _traverse( NODE *root)
{
	if(root != NULL){
		_traverse(root->left);
		printf("%d ", root->data);
		root->height = getHeight(root);
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree)
{
	_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
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

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root)
{
	int leftH;
	int rightH;

	if(root == NULL){
		return 0;
	}
	leftH = getHeight(root->left);
	rightH = getHeight(root->right);

	return max(leftH, rightH)+1;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root)
{
	NODE* imsi = root->left->right;
	root->left->right = root;
	root = root->left;
	root->right->left = imsi;

	root->height = getHeight(root);
	root->right->height = getHeight(root->right);
	if(root->right->left != NULL){
		root->right->left->height = getHeight(root->right->left);
	}

	return root;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root)
{
	NODE* imsi = root->right->left;
	root->right->left = root;
	root = root->right;
	root->left->right = imsi;

	root->height = getHeight(root);
	root->left->height = getHeight(root->left);
	if(root->left->right != NULL){
		root->left->right->height = getHeight(root->left->right);
	}	

	return root;
}