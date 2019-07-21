#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	char 			*entry;
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void);

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root);

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str);

/* Retrieve trie for the requested key
	return	1 key found
			0 key not found
*/
int trieSearch( TRIE *root, char *str);

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root);

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
	using trieList function
*/
void triePrefixList( TRIE *root, char *str);

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]);

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size);

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str);

int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	printf( "Inserting to trie...\t");
	while (fscanf( fp, "%s", str) == 1) // words file
	{	
		ret = trieInsert( trie, str);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);
			
			for (int i = 0; i < num_p; i++)
				trieInsert( permute_trie, permuterms[i]);
			
			clear_permuterms( permuterms, num_p);
		}
	}
	
	printf( "[done]\n"); // Inserting to trie

	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) == 1)
	{
		if (strchr( str, '*')) // wildcard search term
		{
			trieSearchWildcard( permute_trie, str);
		}
		else // search term
		{
			ret = trieSearch( trie, str);
			printf( "[%s]%s found!\n", str, ret ? "": " not");
		}
		printf( "\nQuery: ");
	}

	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}

TRIE *trieCreateNode(void)
{
	TRIE* newTrie = (TRIE*)malloc(sizeof(TRIE));
	if(newTrie == NULL){
		return NULL;
	}

	for(int i=0;i<MAX_DEGREE;i++){
		newTrie->subtrees[i] = NULL;
	}
	newTrie->entry = NULL;

	return newTrie;
}

void trieDestroy( TRIE *root)
{
	if(root != NULL){
		for(int i=0;i<MAX_DEGREE;i++){
			trieDestroy(root->subtrees[i]);
		}
		if(root->entry != NULL){
			free(root->entry);
			root->entry = NULL;
		}
		free(root);
		root = NULL;
	}
}

int trieInsert( TRIE *root, char *str)
{
	TRIE* curNode = root;
	int length = strlen(str);

	for(int j=0;j<length;j++){
		str[j] = tolower(str[j]);
		int index = getIndex(str[j]);
		if(index < 0 || index > MAX_DEGREE-1){
			return 0;
		}
	}

	if(trieSearch(root, str)){
		return 0;
	}

	for(int i=0;i<length;i++){
		int index = getIndex(str[i]);
		if(curNode->subtrees[index] == NULL){
			TRIE* newNode = trieCreateNode();

			curNode->subtrees[index] = newNode;
			curNode = curNode->subtrees[index];
		}
		else{
			curNode = curNode->subtrees[index];
		}
	}
	char* new = (char*)malloc(sizeof(char)*(length+1));
	strcpy(new, str);
	new[length] = '\0';
	curNode->entry = new;

	return 1;
}

int trieSearch( TRIE *root, char *str)
{
	TRIE* curNode = root;
	for(int i=0;i<strlen(str);i++){
		str[i] = tolower(str[i]);
		int index = getIndex(str[i]);
		if(index >= 0 && index <= MAX_DEGREE-1){
			if(curNode->subtrees[index] == NULL){
				return 0;
			}
			else{
				curNode = curNode->subtrees[index];
			}
		}
		else{
			return 0;
		}
	}
	if(curNode->entry != NULL){
		return 1;
	}
	else{
		return 0;
	}
}

void trieList( TRIE *root)
{
	if(root->entry != NULL){
		printf("%s\n", root->entry);	
	}
	for(int i=0;i<MAX_DEGREE;i++){
		if(root->subtrees[i] != NULL)
			trieList(root->subtrees[i]);
	}
	
}

void triePrefixList( TRIE *root, char *str)
{
	TRIE* curNode = root;
	int success = 1;
	for(int i=0;i<strlen(str);i++){
		int index = getIndex(str[i]);
		if(index >= 0 && index <= MAX_DEGREE-1){
			if(curNode->subtrees[index] == NULL){
				success = 0;
				break;
			}
			else{
				curNode = curNode->subtrees[index];
			}
		}
		else{
			success = 0;
			break;
		}
	}
	if(success == 1){
		trieList(curNode);
	}
}

int make_permuterms( char *str, char *permuterms[])
{
	int length = strlen(str);
	int n = 1;

	permuterms[0] = (char*)malloc(sizeof(char)*(length+2));
    strcat(str, "$");
    strcpy(permuterms[0], str);
    permuterms[0][length+1] = '\0';

	for(int i=1;i<=length;i++){
		permuterms[i] = (char*)malloc(sizeof(char)*(length+2));
		strcpy(permuterms[i], &(permuterms[i-1][1]));
		permuterms[i][length] = permuterms[i-1][0];
        permuterms[i][length+1] = '\0';

		n++;
	}
	
	return n;
}

void clear_permuterms( char *permuterms[], int size)
{
	for(int i=0;i<size;i++){
		free(permuterms[i]);
		permuterms[i] = NULL;
	}
}

void trieSearchWildcard( TRIE *root, char *str)
{
	char* permuterm = (char*)malloc(sizeof(char)*(strlen(str)+2));
	strcpy(permuterm, strchr(str, '*')+sizeof(char));
    strcat(permuterm, "$");
    int index = (int)((strchr(str, '*')-str)/sizeof(char));
    strncat(permuterm, str, index);
	permuterm[strlen(str)+1] = '\0';

	for(int i=0;i<strlen(permuterm);i++){
		permuterm[i] = tolower(permuterm[i]);
	}

	triePrefixList(root, permuterm);
	free(permuterm);
}
