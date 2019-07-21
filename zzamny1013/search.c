//#define DEBUG 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "trie.h"

// 역색인 헤더 정보에 대한 구조체
typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// document frequency
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 헤더 정보가 저장된 파일(예) "header.idx")을 읽어 메모리에 저장한다.
// 헤더 구조체 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환

tHEADER *load_header( char *filename);

// 포스팅 리스트가 저장된 파일(예) "posting.idx")를 읽어 메모리에 저장한다.
// 포스팅 리스트(int arrary) 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
int *load_posting( char *filename);
// 문서 집합을 화면에 출력한다.
void showDocuments( int *docs, int numdocs);

// 두 문서 집합의 교집합을 구한다.
// 교집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 교집합의 문서 수는 newnumdocs에 저장한다.
int *intersectDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs);

// 두 문서 집합의 합집합을 구한다.
// 합집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 합집합의 문서 수는 newnumdocs에 저장한다.
int *unionDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs);

// 단일 텀(single term)을 검색하여 문서를 찾는다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *getDocuments( tHEADER *header, int *posting, TRIE *trie, char *term, int *numdocs);

// 질의(query)를 검색하여 문서를 찾는다.
// 질의는 단일 텀 또는 하나 이상의 불린 연산자('&' 또는 '|')를 포함한 질의가 될 수 있다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *searchDocuments( tHEADER *header, int *posting, TRIE *trie, char *query, int *numdocs);

////////////////////////////////////////////////////////////////////////////////
static char *rtrim( char *str);

static char *ltrim( char *str);

static char *trim( char *str);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tHEADER *header;
	int *posting;
	TRIE *trie;
	char query[100];
	int index;
	
	header = load_header( "header.idx");
	if (header == NULL) return 1;
	
	posting = load_posting( "posting.idx");
	if (posting == NULL) return 1;
		
	trie = dic2trie( "dic.txt");
	
	printf( "\nQuery: ");
	while (fgets( query, 100, stdin) != NULL)
	{
		int numdocs;
		int *docs = searchDocuments( header, posting, trie, query, &numdocs); 
		
		if (docs == NULL) printf( "not found!\n");
		else 
		{
			showDocuments( docs, numdocs);
			free( docs);
		}
		printf( "\nQuery: ");
	}
	
	free( header);
	free( posting);
	trieDestroy( trie);
	
	return 0;
}

tHEADER *load_header( char *filename)
{
	FILE* fp;
	fp = fopen(filename, "rb");

	if(fp == NULL){
		return NULL;
	}
	
	tHEADER* headers = (tHEADER*)malloc(sizeof(tHEADER)*25700);
	fread(headers, sizeof(tHEADER), 25700, fp);

	fclose(fp);
	return headers;
}

int *load_posting( char *filename)
{
	FILE* fp;
	fp = fopen(filename, "rb");

	if(fp == NULL){
		return NULL;
	}
	
	int* postings = (int*)malloc(sizeof(int)*410000);
	fread(postings, sizeof(int), 410000, fp);
	fclose(fp);
	return postings;	
}

void showDocuments( int *docs, int numdocs)
{
	for(int i=0;i<numdocs;i++){
		printf(" %d", docs[i]);
	}
}

int *intersectDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs)
{
	int i1 = 0;
	int i2 = 0;
	int i3 = 0;

	if(docs == NULL || docs2 == NULL || numdocs == 0 || numdocs2 == 0){
		free(docs);
		free(docs2);
		return NULL;
	}

	int* new = (int*)malloc(sizeof(int)*500);

	if(new == NULL){
		return NULL;
	}

	while(i1 < numdocs && i2 < numdocs2){
		if(docs[i1]>docs2[i2]){
			i2++;
		}
		else if(docs[i1]<docs2[i2]){
			i1++;
		}
		else{
			new[i3] = docs[i1];
			i1++;
			i2++;
			i3++;
		}
	}
	free(docs);
	free(docs2);
	*newnumdocs = i3;
	return new;
}

int *unionDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs)
{
	int i1 = 0;
	int i2 = 0;
	int i3 = 0;

	int* new = (int*)malloc(sizeof(int)*2000);

	if(new == NULL){
		return NULL;
	}

	while(i1 < numdocs && i2 < numdocs2){
		if(docs[i1]>docs2[i2]){
			new[i3] = docs2[i2];
			i3++;
			i2++;
		}
		else if(docs[i1]<docs2[i2]){
			new[i3] = docs[i1];
			i3++;
			i1++;
		}
		else{
			new[i3] = docs[i1];
			i1++;
			i2++;
			i3++;
		}
	}

	if(i1 == numdocs){
		for(;i2<numdocs2;i2++){
			new[i3] = docs2[i2];
			i3++;
		}
	}
	else if(i2 == numdocs2){
		for(;i1<numdocs;i1++){
			new[i3] = docs[i1];
			i3++;
		}
	}
	free(docs);
	free(docs2);
	*newnumdocs = i3;
	return new;
}

int *getDocuments( tHEADER *header, int *posting, TRIE *trie, char *term, int *numdocs)
{
	int dic_index = trieSearch(trie, term);
	if(dic_index == -1){
		return NULL;
	}

	int header_index = header[dic_index].index;
	int header_df = header[dic_index].df;

	int* doc_array = (int*)malloc(sizeof(int)*header_df);
	
	for(int i=0;i<header_df;i++){
		doc_array[i] = posting[header_index+i];
	}

	*numdocs = header_df;
	return doc_array;
}

int *searchDocuments( tHEADER *header, int *posting, TRIE *trie, char *query, int *numdocs)
{
	query = trim(query);

	char* term;
	int* result = NULL;
	int numdocs1 = 0;
	int numdocs2 = 0;
	int recursive = 0;

	for(int i=strlen(query)-1;i>=0;i--){
		query[i] = tolower(query[i]);
		if(query[i] == '|'){

			term = &query[i+1];
			term = trim(term);
			query[i] = '\0';
			query = trim(query);

			int* getDoc = getDocuments(header, posting, trie, term, &numdocs1);

			int* recDoc = searchDocuments(header, posting, trie, query, &numdocs2);

			return unionDocuments(recDoc, numdocs2, getDoc, numdocs1, numdocs);

		}
		else if(query[i] == '&'){
			term = &query[i+1];
			term = trim(term);
			query[i] = '\0';
			query = trim(query);
			
			int* getDoc = getDocuments(header, posting, trie, term, &numdocs1);

			int* recDoc = searchDocuments(header, posting, trie, query, &numdocs2);

			return intersectDocuments(recDoc, numdocs2, getDoc, numdocs1, numdocs);

		}
	}

	return getDocuments(header, posting, trie, query, numdocs);

}

////////////////////////////////////////////////////////////////////////////////
static char *rtrim( char *str)
{
	char *p = str + strlen( str) - 1;
	
	while (p >= str)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') *p = 0;
		else return str;
		p--;
	}
}

static char *ltrim( char *str)
{
	char *p = str;
	
	while (*p)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') p++;
		else 
		{
			return p;
		}
	}
	return NULL;
}

static char *trim( char *str)
{
	if( str == NULL || *str == 0) return str;
	
	return rtrim(ltrim(str));
}