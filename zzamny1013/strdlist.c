#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define DELETE			4

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
	struct node	*llink;
	struct node	*rlink;
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
LIST *createList( void);

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList);

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr);

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* returns number of nodes in list
*/
int listCount( LIST *pList);

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList);

//int fullList( LIST *pList);

/* prints data from list (forward)
*/
void printList( LIST *pList);

/* prints data from list (backward)
*/
void printListR( LIST *pList);

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr);

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr);

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str);

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken);

/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'D':
			return DELETE;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%s", str) == 1)
	{
		pToken = createToken( str);
		
		// insert function call
		ret = addNode( list, pToken);
		
		if (ret == 1) // duplicated 
			destroyToken( pToken);
	}
	
	fclose( fp);
	
	fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
	
	while (1)
	{
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				printList( list);
				break;
			
			case BACKWARD_PRINT:
				printListR( list);
				break;
			
			case DELETE:
				fprintf( stdout, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				int ret = removeNode( list, str, &pToken);
				if (ret)
				{
					fprintf( stdout, "%s deleted\n", pToken->token);
					destroyToken( pToken);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
			}
		
		if (action) fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
	}
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
	pList->pos = pList->head->rlink;
	while (pList->head->rlink != NULL) {
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

int addNode( LIST *pList, tTOKEN *dataInPtr){
    if(pList->head == NULL){
        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if(newNode == NULL)
            return -1;
        
        newNode->dataPtr = dataInPtr;
        pList->head = newNode;
        newNode->llink = NULL;
        newNode->rlink = NULL;
        pList->rear = newNode;
		pList->pos = newNode;
        (pList->count)++;

		return 0;
    }
	if(strcmp(pList->head->dataPtr->token, dataInPtr->token) > 0){
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if(newNode == NULL)
            return -1;
		
		newNode->dataPtr = dataInPtr;
		pList->head->llink = newNode;
		newNode->llink = NULL;
		newNode->rlink = pList->head;
		pList->head = newNode;
		pList->pos = newNode;
		(pList->count)++;

		return 0;

	}
	if(strcmp(pList->rear->dataPtr->token, dataInPtr->token) < 0){
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if(newNode == NULL)
            return -1;

		newNode->dataPtr = dataInPtr;
		newNode->rlink = NULL;
		pList->rear->rlink = newNode;
		newNode->llink = pList->rear;
		pList->rear = newNode;
		pList->pos = newNode;
		(pList->count)++;
		return 0;
	}
    int whereareyou = _search(pList, &(pList->head), &(pList->head->rlink), dataInPtr->token);
	if(whereareyou == 1){
		(pList->pos->dataPtr->freq)++;

		return 1;
	}
	else{
		int test = _insert(pList, pList->pos, dataInPtr);
		if(!test)
			return -1;
		
		return 0;
	}
}

int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut){
	if(!strcmp(pList->head->dataPtr->token, keyPtr)){
		NODE* curNode;
		curNode = pList->head;
		pList->head = pList->head->rlink;
		*dataOut = curNode->dataPtr;

		free(curNode);
		pList->pos = pList->head;
		(pList->count)--;

		return 1;
	}
	if(_search(pList, &(pList->head), &(pList->head->rlink), keyPtr)){
		_delete(pList, pList->pos->llink, pList->pos, dataOut);

		return 1;
	}
	else{
		return 0;
	}
}

int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut){
	if(_search(pList, &(pList->head), &(pList->head->rlink), pArgu)){
		*pDataOut = pList->pos->dataPtr;
		return 1;
	}

	return 0;
}

int listCount( LIST *pList){
	return pList->count;
}

int emptyList( LIST *pList){
	int rtrn = (pList->count)>0 ? 0 : 1;

	return rtrn;
}

void printList(LIST *pList) {
	NODE* curNode;
	curNode = pList->head;
	while (curNode != NULL) {
		printf("%s\t%d\n", curNode->dataPtr->token, curNode->dataPtr->freq);
		curNode = curNode->rlink;
	}

}

void printListR(LIST *pList) {
	NODE* curNode;
	curNode = pList->rear;
	while (curNode != NULL) {
		printf("%s\t%d\n", curNode->dataPtr->token, curNode->dataPtr->freq);
		curNode = curNode->llink;
	}

}

static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr){
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if(newNode == NULL)
        return 0;
    
    newNode->dataPtr = dataInPtr;
    newNode->rlink = pPre->rlink;
    pPre->rlink->llink = newNode;
    pPre->rlink = newNode;
    newNode->llink = pPre;

    (pList->count)++;
    pList->pos = newNode;

    return 1;

}

static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr){
	if(pLoc->rlink == NULL){
		*dataOutPtr = pLoc->dataPtr;
		
		free(pLoc);
		pPre->rlink = NULL;
		pList->rear = pPre;
		pList->pos = pList->rear;
		(pList->count)--;
	}

	else{
		pPre->rlink = pLoc->rlink;
		pLoc->rlink->llink = pPre;
		*dataOutPtr = pLoc->dataPtr;
		
		free(pLoc);
		(pList->count)--;
		pList->pos = pPre->rlink;
	}
}

static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu){
    pList->pos = pList->head;
	int pcmp;
	int lcmp;
	if(strcmp(pList->head->dataPtr->token, pArgu) == 0){
		return 1;
	}
    while(pList->pos != NULL){
        if(strcmp(pArgu, (*pPre)->dataPtr->token) > 0 && strcmp(pArgu, (*pLoc)->dataPtr->token) <= 0){
            if(strcmp(pArgu, (*pLoc)->dataPtr->token) == 0){
                pList->pos = *pLoc;

                return 1;
            }

            return 0;
        }
        else{
            pList->pos = pList->pos->rlink;
            pPre = &((*pPre)->rlink);
            pLoc = &((*pLoc)->rlink);
        }
    }

    return 0;
}

tTOKEN *createToken( char *str){
    tTOKEN* newToken = (tTOKEN*)malloc(sizeof(tTOKEN));
    if(newToken == NULL)
        return NULL;
    newToken->token = strdup(str);
    newToken->freq = 1;

    return newToken;
}

tTOKEN *destroyToken( tTOKEN *pToken){
    free(pToken->token);
    free(pToken);

    return pToken;
}