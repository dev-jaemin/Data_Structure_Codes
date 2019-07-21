


#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW



// TRIE type definition
typedef struct trieNode {
	int 			index; // 0, 1, 2, ...
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
/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str);

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);



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

/* makes a trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
TRIE *dic2trie( char *dicfile);


/* makes a permuterm trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
TRIE *dic2permute_trie( char *dicfile);
