#include <stdlib.h> // malloc
#include <stdio.h>
#include <ctype.h> // toupper

#define QUIT	1
#define INSERT	2
#define DELETE	3
#define PRINT	4
#define SEARCH	5

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	int			data;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

static int _insert(LIST *pList, NODE *pPre, int dataIn);
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, int *dataOut);
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, int argu);

/* Allocates dynamic memory for a list head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
LIST *createList(void) {
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	if (pList == NULL)
		return NULL;
	pList->count = 0;
	pList->pos = NULL;
	pList->head = NULL;
	pList->rear = NULL;

	return pList;
}

/* Deletes all data in list and recycles memory
return	NULL head pointer
*/
LIST *destroyList(LIST *pList) {
	int n;
	if (pList == NULL) {
		return pList;
	}
	pList->pos = pList->head->link;
	while (pList->head->link != NULL) {
		_delete(pList, pList->head, pList->pos, &n);
	}
	pList->count = 0;
	pList->head = NULL;
	pList->rear = NULL;
	pList->pos = NULL;
	free(pList);
	
	return pList;
}

/* Inserts data into list
return	-1 if overflow
0 if successful
1 if dupe key
*/
//pos이용하여 sorting을 여기서
//_search로 넣을 자리 찾고(pos 설정해놓고)
//_insert로 집어넣고(pPre에 pos를 인자로 놓는다)
int addNode(LIST *pList, int dataIn) {
	if (pList->head->data > dataIn) {
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		newNode->data = dataIn;
		newNode->link = pList->head;
		pList->head = newNode;
	}
	if (_search(pList, &(pList->head), &(pList->head->link), dataIn)) {
		return 1;
	}
	else {
		int test = _insert(pList, pList->pos, dataIn);
		if (!test)
			return -1;
		return 0;
	}
}

/* Removes data from list
return	0 not found
1 deleted
*/
//pos=pPre임
int removeNode(LIST *pList, int Key, int *dataOut) {
	if (pList->head->data == Key) {
		NODE* curNode = (NODE*)malloc(sizeof(NODE));
		curNode = pList->head;
		pList->head = pList->head->link;
		free(curNode);
	}
	if (_search(pList, &(pList->head), &(pList->head->link), Key)) {
		_delete(pList, pList->pos, pList->pos->link, dataOut);
	}
	else
		return 0;
}

/* interface to search function
Argu	key being sought
dataOut	contains found data
return	1 successful
0 not found
*/
int searchList(LIST *pList, int Argu, int *dataOut) {
	if (_search(pList, &(pList->head), &(pList->head->link), Argu)) {
		return 1;
	}
	else {
		return 0;
	}
}

/* returns number of nodes in list
*/
int listCount(LIST *pList) {
	return pList->count;
}

/* returns	1 empty
0 list has data
*/
int emptyList(LIST *pList) {
	int rtrn = (pList->count)>0 ? 0 : 1;

	return rtrn;
}

int fullList( LIST *pList){
	int rtrn = (pList->count)>0 ? 1: 0;
	
	return rtrn;
}

/* prints data from list
*/
void printList(LIST *pList) {
	NODE* curNode = (NODE*)malloc(sizeof(NODE));
	curNode = pList->head;
	if (pList->head == NULL)
		printf("NULL\n");
	else {
		while (curNode != NULL) {
			printf("%d->", curNode->data);
			curNode = curNode->link;
		}
		printf("NULL\n");
	}
}

/* internal insert function
inserts data into a new node
return	1 if successful
0 if memory overflow
*/
//_search호출
static int _insert(LIST *pList, NODE *pPre, int dataIn) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode == NULL)
		return 0;
	newNode->data = dataIn;
	newNode->link = pPre->link;
	pPre->link = newNode;
	pList->pos = pList->pos->link;
	(pList->count)++;

	return 1;
}

/* internal delete function
deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, int *dataOut) {
	NODE* curNode = (NODE*)malloc(sizeof(NODE));
	curNode = pLoc;
	pPre->link = pLoc->link;
	*dataOut = curNode->data;
	free(curNode);
	pList->pos = pPre->link;
	(pList->count)--;
}


/* internal search function
searches list and passes back address of node
containing target and its logical predecessor
return	1 found
0 not found
*/
//pPre를 pos로 두고 그 다음(pLoc)이랑 비교해서 그 사이를 찾아낸다.
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, int argu) {
	pList->pos = pList->head;
	if (pList->pos->data == argu)
		return 1;
	while (pList->pos != NULL) {
		if ((*pPre)->data <= argu && (*pLoc)->data > argu) {
			if (((*pLoc)->data) == argu) {
				return 1;
			}
			return 0;
		}
		else {
			pList->pos = pList->pos->link;
			*pPre = (*pPre)->link;
			*pLoc = (*pLoc)->link;
		}
	}

	return 0;
}


/* gets user's input
*/
int get_action()
{
	char ch;

	scanf("%c", &ch);
	ch = toupper(ch);

	switch (ch)
	{
	case 'Q':
		return QUIT;
	case 'P':
		return PRINT;
	case 'I':
		return INSERT;
	case 'D':
		return DELETE;
	case 'S':
		return SEARCH;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	int num;
	LIST *list;
	int data;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}

	fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");

	while (1)
	{
		int action = get_action();

		switch (action)
		{
		case QUIT:
			destroyList( list);
			return 0;

		case PRINT:
			// print function call
			printList( list);
			break;

		case INSERT:
			fprintf(stdout, "Enter a number to insert: ");
			fscanf(stdin, "%d", &num);

			// insert function call
			addNode( list, num);

			// print function call
			printList( list);
			break;

		case DELETE:
			fprintf(stdout, "Enter a number to delete: ");
			fscanf(stdin, "%d", &num);

			// delete function call
			removeNode( list, num, &data);

			// print function call
			printList( list);
			break;

		case SEARCH:
			fprintf(stdout, "Enter a number to retrieve: ");
			fscanf(stdin, "%d", &num);

			// search function call
			int found;
			found = searchList( list, num, &data);
			if (found) fprintf( stdout, "Found: %d\n", data);
			else fprintf( stdout, "Not found: %d\n", num);

			break;
		}
		if (action) fprintf(stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");

	}

	return 0;
}