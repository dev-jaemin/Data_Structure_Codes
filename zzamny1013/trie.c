#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW '$'
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))


// TRIE type definition

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void)
{
	TRIE* newTrie = (TRIE*)malloc(sizeof(TRIE));
	if(newTrie == NULL){
		return NULL;
	}

	for(int i=0;i<MAX_DEGREE;i++){
		newTrie->subtrees[i] = NULL;
	}
	newTrie->index = -1;

	return newTrie;
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root)
{
	if(root != NULL){
	for(int i=0;i<MAX_DEGREE;i++){
		trieDestroy(root->subtrees[i]);
	}
	free(root);
	root = NULL;
	}
}

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str)
{
	TRIE* curNode = root;
	for(int i=0;i<strlen(str);i++){
		str[i] = tolower(str[i]);
		int index = getIndex(str[i]);
		if(index >= 0 && index <= MAX_DEGREE-1){
			if(curNode->subtrees[index] == NULL){
				return -1;
			}
			else{
				curNode = curNode->subtrees[index];
			}
		}
		else{
			return -1;
		}
	}
	if(curNode->index != -1){
		return curNode->index;
	}
	else{
		return -1;
	}
}

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index)
{
	TRIE* curNode = root;
	int length = strlen(str);

	if(trieSearch(root, str) != -1){
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
	curNode->index = dic_index;

	return 1;
}



/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root)
{
	if(root->index != -1){
		printf("%d\n", root->index);	
	}
	for(int i=0;i<MAX_DEGREE;i++){
		if(root->subtrees[i] != NULL)
			trieList(root->subtrees[i]);
	}
	
}

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
	using trieList function
*/
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

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
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

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size)
{
	for(int i=0;i<size;i++){
		free(permuterms[i]);
		permuterms[i] = NULL;
	}
}

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
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

/* makes a trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
TRIE *dic2trie( char *dicfile)
{
	printf( "Inserting to trie...\t");
	FILE* dic_File = fopen(dicfile, "r");
	char line[30];
	int index = 0;
	TRIE* trie = trieCreateNode();

	while(fscanf(dic_File, "%s", line) != -1){
		trieInsert(trie, line, index);
		index++;
	}

	fclose(dic_File);
	printf( "[done]\n");

	return trie;
}


/* makes a permuterm trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
//TRIE *dic2permute_trie( char *dicfile);
