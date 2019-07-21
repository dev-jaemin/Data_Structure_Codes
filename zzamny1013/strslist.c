#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
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

/* Allocates dynamic memory for a list head node and returns its address to caller
return	head node pointer
NULL if overflow
*/
LIST *createList(void);

/* Deletes all data in list and recycles memory
return	NULL head pointer
*/
LIST *destroyList(LIST *pList);

/* Inserts data into list
return	-1 if overflow
0 if successful
1 if duplicated key
*/
int addNode(LIST *pList, tTOKEN *dataInPtr);

/* Removes data from list
return	0 not found
1 deleted
*/
int removeNode(LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
Argu	key being sought
dataOut	contains found data
return	1 successful
0 not found
*/
int searchList(LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* returns number of nodes in list
*/
int listCount(LIST *pList);

/* returns	1 empty
0 list has data
*/
int emptyList(LIST *pList);

//int fullList( LIST *pList);

/* prints data from list
*/
void printList(LIST *pList);

/* internal insert function
inserts data into a new node
return	1 if successful
0 if memory overflow
*/
static int _insert(LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

/* internal search function
searches list and passes back address of node
containing target and its logical predecessor
return	1 found
0 not found
*/
static int _search(LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
return	token structure pointer
NULL if overflow
*/
tTOKEN *createToken(char *str);

/* Deletes all data in token structure and recycles memory
return	NULL head pointer
*/
tTOKEN *destroyToken(tTOKEN *pToken);

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;

	// creates a null list
	list = createList();
	if (!list)
	{
		printf("Cannot create list\n");
		return 100;
	}

	while (scanf("%s", str) == 1)
	{
		pToken = createToken(str);

		// insert function call
		ret = addNode(list, pToken);

		if (ret == 1) // duplicated 
			destroyToken(pToken);
	}
	// print function call
	printList(list);

	destroyList(list);

	return 0;
}


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

LIST *destroyList(LIST *pList) {
	tTOKEN* t;
	if (pList == NULL) {
		return pList;
	}
	pList->pos = pList->head->link;
	while (pList->head->link != NULL) {
		_delete(pList, pList->head, pList->pos, &t);
		free(t->token);
		free(t);
		
	}
	free(pList->head->dataPtr->token);
	free(pList->head->dataPtr);
	free(pList->head);
	free(pList);

	return pList;
}

int addNode(LIST *pList, tTOKEN *dataInPtr) {
	if (pList->head == NULL) {
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if (newNode == NULL)
			return -1;
		newNode->dataPtr = dataInPtr;
		newNode->link = NULL;
		pList->head = newNode;
		pList->rear = newNode;
		pList->pos = newNode;
		(pList->count)++;

		return 0;
	}
	if (strcmp(pList->head->dataPtr->token, dataInPtr->token) > 0) {
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if (newNode == NULL)
			return -1;
		newNode->dataPtr = dataInPtr;
		newNode->link = pList->head;
		pList->head = newNode;
		pList->pos = newNode;
		(pList->count)++;

		return 0;
	}
	if (strcmp(pList->rear->dataPtr->token, dataInPtr->token) < 0) {
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if (newNode == NULL)
			return -1;
		newNode->dataPtr = dataInPtr;
		newNode->link = NULL;
		pList->rear->link = newNode;
		pList->rear = newNode;
		pList->pos = newNode;
		(pList->count)++;

		return 0;
	}
	if (strcmp(pList->head->dataPtr->token, dataInPtr->token) == 0) {
		(pList->head->dataPtr->freq)++;

		return 1;
	}
	if (_search(pList, &(pList->head), &(pList->head->link), dataInPtr->token)) {
			(pList->pos->link->dataPtr->freq)++;

			return 1;
	}
	else {
			int test = _insert(pList, pList->pos, dataInPtr);
			if (!test)
				return -1;

			return 0;
	}


}

int removeNode(LIST *pList, char *keyPtr, tTOKEN **dataOut) {
	if (pList->head == NULL) {
		return 0;
	}
	if (!strcmp(pList->head->dataPtr->token, keyPtr)) {
		NODE* curNode;
		curNode = pList->head;
		*dataOut = curNode->dataPtr;
		pList->head = pList->head->link;
	
		free(curNode);
		pList->pos = pList->head;
		(pList->count)--;

		return 1;
	}
	if (_search(pList, &(pList->head), &(pList->head->link), keyPtr)) {
		_delete(pList, pList->pos, pList->pos->link, dataOut);

		return 1;
	}

	return 0;
}

int searchList(LIST *pList, char *pArgu, tTOKEN **pDataOut){
	if(_search(pList,&(pList->head),&(pList->head->link), pArgu)){
		(*pDataOut)->token = strdup(pArgu);

		return 1;
	}

	else
		return 0;
}

int listCount(LIST *pList){
	return pList->count;
}

int emptyList(LIST *pList) {
	int rtrn = (pList->count)>0 ? 0 : 1;

	return rtrn;
}

//int fullList( LIST *pList);

void printList(LIST *pList) {
	NODE* curNode;
	curNode = pList->head;
	while (curNode != NULL) {
		printf("%s\t%d\n", curNode->dataPtr->token, curNode->dataPtr->freq);
		curNode = curNode->link;
	}

}

static int _insert(LIST *pList, NODE *pPre, tTOKEN *dataInPtr) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode == NULL) {
		return 0;
	}
	newNode->dataPtr = dataInPtr;
	newNode->link = pPre->link;
	pPre->link = newNode;
	pList->pos = newNode;
	(pList->count)++;

	return 1;
}

static void _delete(LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr) {
	if(pLoc->link == NULL){
		*dataOutPtr = pLoc->dataPtr;
		free(pLoc);
		pPre->link = NULL;
		pList->rear = pPre;
		pList->pos = pList->rear;
		(pList->count)--;
	}

	else{
		*dataOutPtr = pLoc->dataPtr;
		pPre->link = pLoc->link;
		
		free(pLoc);
		(pList->count)--;
		pList->pos = pPre->link;
	}

}

static int _search(LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu) {	
	if (pList->head == NULL)
		return 0;
	pList->pos = pList->head;
	int pcmp;
	int lcmp;
	while (pList->pos != NULL) {
		pcmp = strcmp((*pPre)->dataPtr->token, pArgu);
		lcmp = strcmp(pArgu, (*pLoc)->dataPtr->token);
		if (pcmp < 0 && lcmp <= 0) {
			if (lcmp == 0) {
				return 1;
			}
			return 0;
		}
		else{
			pPre = &((*pPre)->link);
			pLoc = &((*pLoc)->link);
			pList->pos = pList->pos->link;
		}
	}

	return 0;

}

tTOKEN *createToken(char *str) {
	tTOKEN* newToken = (tTOKEN*)malloc(sizeof(tTOKEN));
	if (newToken == NULL)
		return NULL;
	newToken->token = strdup(str);
	newToken->freq = 1;

	return newToken;
}

tTOKEN *destroyToken(tTOKEN *pToken) {
	free(pToken->token);
	free(pToken);

	return pToken;
}