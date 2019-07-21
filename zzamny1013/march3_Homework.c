#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
	int data;
	struct _node* next;
} Node;

void change(Node* a, Node* b){
	if(a->data > b->data){
		int tmp = a->data;
		a->data = b->data;
		b->data = tmp;
	}
}

int main(){
	Node* head = NULL;
	Node* tail = NULL;
	Node* cur = NULL;
	
	Node* newNode = NULL;
	int readData;
	
	while(1){
		printf("자연수 입력: ");
		scanf("%d", &readData);
		if(readData<1)
			break;
		
		newNode = (Node*)malloc(sizeof(Node));
		newNode->data = readData;
		newNode->next = NULL;
		
		if(head == NULL)
			head = newNode;
		else
			tail->next = newNode;
		
		tail = newNode;
	}
	printf("\n\n");
	
	printf("sorting을 해볼게요.\n");
	if(head == NULL)
		printf("저장된 데이터가 없습니다.\n");
	else{
		while(cur->next != NULL){
			cur = head;
			while(cur->next != NULL){
				change(cur, cur->next);
			}
		}
	}
	
	printf("입력 받은 데이터의 전체 출력!\n");
	
	if(head == NULL)
		printf("저장된 데이터가 없습니다\n");
	else{
		cur = head;
		printf("%d -> ", cur->data);
		
		while(cur->next != NULL){
			cur = cur->next;
			printf("%d-> ", cur->data);
		}
	}
	
	
	
	if(head == NULL)
		return 0;
	else{
		Node* delNode = head;
		Node* delNextNode = head->next;
		
		printf("%d를 삭제합니다.\n", delNode->data);
		free(delNode);
		
		while(delNextNode !=NULL)
		{
			delNode = delNextNode;
			delNextNode = delNode->next;
			
			printf("%d를 삭제합니다. \n", delNode->data);
			free(delNode);
		}
	}
	return 0;
}