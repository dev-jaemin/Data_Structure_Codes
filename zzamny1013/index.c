#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// 토큰-문서 구조체
typedef struct {
	char	*token;	// 토큰
	int		docid;	// 문서번호(document ID)
} tTokenDoc;

typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// 문서 빈도(document frequency)
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 토큰 구조체로부터 역색인 파일을 생성한다.
void invertedIndex( tTokenDoc *tokens, int num_tokens, 
					char *dicfilename, char *headerfilename, char *postingfilename)
{
	FILE* dic_File = fopen(dicfilename, "w");
	FILE* header_File = fopen(headerfilename, "wb");
	FILE* posting_File = fopen(postingfilename, "wb");

	char tab = '\t';
	char enter[] = "\r\n";

	int hi = 0;
	int pi = 0;
	char* prevToken = tokens[pi].token;
	int n = 0;
	int real_count = 1;

	tHEADER* headers = (tHEADER*)malloc(sizeof(tHEADER)*num_tokens);
	int* postings = (int*)malloc(sizeof(int)*num_tokens);

	headers[0].index = 0;
	headers[0].df = 1;

	postings[pi] = tokens[pi].docid;

	for(pi=1;pi<num_tokens;pi++){
		if(strcmp(prevToken, tokens[pi].token) == 0){
			if(tokens[pi-1].docid == tokens[pi].docid){
				n++;
				continue;
			}
	
			postings[real_count] = tokens[pi].docid;
			(headers[hi].df)++;
			prevToken = tokens[pi].token;
			real_count++;
		}	
		else{
	
			headers[hi+1].index = headers[hi].index + headers[hi].df;
	
			postings[real_count] = tokens[pi].docid;
		
			fprintf(dic_File, "%s %s", tokens[pi-1].token, enter);
			hi++;
			headers[hi].df = 1;
			prevToken = tokens[pi].token;
			real_count++;
		}
	}

	fprintf(dic_File, "%s %s", tokens[pi-1].token, enter);


	fwrite(postings, sizeof(int), real_count, posting_File);
	fwrite(headers, sizeof(tHEADER), hi+1, header_File);

	fclose(dic_File);
	fclose(header_File);
	fclose(posting_File);
	free(headers);
	free(postings);
}

// 입력 파일로부터 토큰-문서 구조체를 생성한다.
tTokenDoc *get_tokens(char *filename, int *num_tokens)
{
	FILE* fp = NULL;
	fp = fopen(filename, "r");

	if(fp == NULL){
		fprintf(stderr, "파일 읽기 오류\n");
		return NULL;
	}

	tTokenDoc* tokens = (tTokenDoc*)malloc(sizeof(tTokenDoc)*1000000);
	int id = 1;
	char line[5000];
	char delimeters[] = " ";
	int token_index = 0;
	while(fgets(line, sizeof(line), fp) != NULL){
		char* line_token = strtok(line, delimeters);

		while(line_token != NULL){
			if(strcmp(line_token, "\n") == 0){
				line_token = strtok(NULL, delimeters);
				continue;
			}
			tokens[token_index].token = strdup(line_token);
			tokens[token_index].docid = id;
			
			token_index++;

			line_token = strtok(NULL, delimeters);
		}
		id++;
	}

	*num_tokens = token_index;
	fclose(fp);

	return tokens;
}

// qsort를 위한 비교함수 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
static int _compare(const void *n1, const void *n2)
{
	int cmp = strcmp((*(tTokenDoc*)n1).token, (*(tTokenDoc*)n2).token);
	if(cmp){
		return cmp;
	}

	return ((*(tTokenDoc*)n1).docid - (*(tTokenDoc*)n2).docid);
}

////////////////////////////////////////////////////////////////////////////////
static void print_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		printf( "%s\t%d\n", tokens[i].token, tokens[i].docid);
	}
}

////////////////////////////////////////////////////////////////////////////////
void destroy_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		free( tokens[i].token);
	}
	free( tokens);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tTokenDoc *tokens;
	int num_tokens = 0;

	if (argc != 2)
	{
		printf( "Usage: %s FILE\n", argv[0]);
		return 2;
	}
	
	tokens = get_tokens( argv[1], &num_tokens);
	
	assert( tokens != NULL && num_tokens > 0);

#if DEBUG	
	//print_tokens( tokens, num_tokens);
#endif

	// 정렬 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
	qsort( tokens, num_tokens, sizeof( tTokenDoc), _compare);
	
	invertedIndex( tokens, num_tokens, "dic.txt", "header.idx", "posting.idx");

#if DEBUG	
	print_tokens( tokens, num_tokens);
#endif	

	destroy_tokens( tokens, num_tokens);
	
	return 0;
}

