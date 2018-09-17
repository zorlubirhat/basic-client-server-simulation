#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct stack
{
  char *arr;
  int top;
  int capacity;
};

struct queue
{
	char *arr;
	int front;
	int rear;
	int capacity;
};

struct client
{
	int cNumber;
	struct stack *cStack;
	struct queue *cQueue;
	struct stack *logHist;
};

struct server
{
	int number;
	struct stack *sStack;
	struct queue *sQueue;
	struct stack *logHist;
};

struct clientSize
{
	struct client *arr[1];
};

FILE* openFile(char *);
struct stack *createStack(int);
int isStackFull(struct stack *);
int isStackEmpty(struct stack *);
void pushStack(struct stack *, char);
char popStack(struct stack *);
void displayStack(struct stack *, FILE *);
struct queue *createQueue(int);
int isQueueFull(struct queue *);
int isQueueEmpty(struct queue *);
void enqueue(struct queue *, char);
char dequeue(struct queue *);
struct client *createClient(struct clientSize *, int, int, int);
struct clientSize *createClientArray(int);
struct server *createServer(int, int, int);
int** createArray(char *);
int client_serverSize(char *);

FILE* openFile(char *argv){
	
	FILE *file = fopen(argv, "r");
	if(file == NULL){
		printf("%c not found!", file);
		return 0;
	}
	return file;
}

struct stack *createStack(int capacity) {
  struct stack *s = (struct stack*)malloc(sizeof(struct stack));
  s->arr = (char*)malloc(sizeof(char)*capacity);
  s->top = -1;
  s->capacity = capacity;
  return s;
}

int isStackFull(struct stack *s){
	if(s->top == s->capacity-1){
		return 1;
	}
	return 0;
}

int isStackEmpty(struct stack *s){
	if(s->top == -1){
		return 1;
	}
	return 0;
}

void pushStack(struct stack *s, char item) {
	if(!isStackFull(s)){
		s->top = s->top + 1;
  	s->arr[s->top] = item;
	}
}

char popStack(struct stack *s) {
  if(!isStackEmpty(s)){
  	char item = s->arr[s->top];
		s->top = s->top - 1;
		return item;
	}
}

void displayStack(struct stack *s, FILE* outfile){
	int i;
	if(!isStackEmpty(s)){
		for(i=0;i<=s->top;i++){
			fprintf(outfile, "%c ", s->arr[i]);
		}
	}
	fprintf(outfile, "\n");
}

struct queue *createQueue(int capacity) {
	struct queue *q = (struct queue*)malloc(sizeof(struct queue));
	q->arr = (char*)malloc(sizeof(char)*capacity);
	q->front = -1;
	q->rear = -1;
	q->capacity = capacity;
}

int isQueueFull(struct queue *q){
	if((q->front == q->rear + 1) || (q->front == 0 && q->rear == (q->capacity)-1)){
		return 1;
	}
	return 0;	
}

int isQueueEmpty(struct queue *q){
	if(q->front == -1){
		return 1;
	}
	return 0;
}

void enqueue(struct queue *q, char item){
	if(!isQueueFull(q)){
		if(q->front == -1){
			q->front = 0;
		}
		q->rear = (q->rear + 1) % q->capacity;
		q->arr[q->rear] = item;
	}
}

char dequeue(struct queue *q){
	if(!isQueueEmpty(q)){
		char item = q->arr[q->front];
		if(q->front == q->rear){
			q->front = -1;
			q->rear = -1;
		}
		else{
			q->front = (q->front + 1) % q->capacity;
		}
		return item;
	}
}

struct client *createClient(struct clientSize *cArray, int number, int stackSize, int queueSize){
	struct client *c = (struct client*)malloc(sizeof(struct client));
	c->cNumber = number;
	c->cQueue = createQueue(queueSize);
	c->cStack = createStack(stackSize);
	c->logHist = createStack(1000);
	cArray->arr[number-1] = c;
	return c;
}

struct clientSize *createClientArray(int size){
	struct clientSize *ca = (struct clientSize*)malloc(sizeof(struct clientSize));
	ca->arr[size];
	return ca;
}

struct server *createServer(int number,int stackSize, int queueSize){
	struct server *s = (struct server*)malloc(sizeof(struct server));
	s->sStack = createStack(stackSize);
	s->sQueue = createQueue(queueSize);
	s->number = number;
	s->logHist = createStack(1000);
	return s;
}

int** createArray(char *fileName){
	
	FILE *file = NULL; 
	file = openFile(fileName);
	char firstInt[2];
	char numbers[2];
	int c;
	int colSize = 2;
	
	int ** arr = NULL;
	
	int iRow = 0;
	int iCol = 0;
	
	fscanf(file,"%s",firstInt);
  int rowSize = atoi(firstInt);
	
	arr = (int **)malloc(rowSize * sizeof(int*));
	
	for (iRow = 0; iRow < rowSize; iRow++){
		arr[iRow] = (int *)malloc(colSize * sizeof(int));
	}
	
	int x, y;		
	if(c != EOF){
		for(x = 0; x < rowSize; x++){
			for(y = 0; y < colSize; y++){
				c = fscanf(file, "%s", numbers);
				arr[x][y] = atoi(numbers);
			}
		}
	}
	
  fclose(file);
  return arr;
}

int client_serverSize(char *fileName){
	
	FILE *file = NULL; 
	file = openFile(fileName);
	char firstInt[2];
	fscanf(file,"%s",firstInt);
  int csSize = atoi(firstInt);
  return csSize;
}

void commandParseandWriteFile(char *input1, char *input2, char *output){
	
	FILE* outFile = fopen(output, "w");
	
	int csSize = client_serverSize(input1);
	
	int **arr = createArray(input1);
	
	struct clientSize *clientsArr = NULL;
	clientsArr = createClientArray(csSize-1);
	
	struct server *server = NULL;
		
	int x,y;
	for(x=0;x <csSize;x++){
		for(y=0;y <csSize; y++){
			if((x+1) == csSize){
				server = createServer(csSize, arr[x][1], arr[x][0]);
			}
			else{
				createClient(clientsArr, x+1, arr[x][1], arr[x][0]);
			}
		}
	}
	
	FILE *file2 = NULL;
	file2 = openFile(input2);
	
	int firstline[4];
	char command[2];
	char csNum[3];
	char letter[2];
	
	int c;
	fscanf(file2,"%d", firstline);
	while(c != EOF){
		c = fscanf(file2,"%s %s %s",command,csNum,letter);
		if(c != EOF){
			if(strcmp("A", command) == 0){
				if(!isQueueFull(clientsArr->arr[atoi(csNum)-1]->cQueue)){
					enqueue(clientsArr->arr[atoi(csNum)-1]->cQueue, letter[0]);
				}
				else{
					pushStack(clientsArr->arr[atoi(csNum)-1]->logHist, '1');
				}
			}
			else if(strcmp("I", command) == 0){
				if(atoi(csNum) == csSize){
					if(!isStackFull(server->sStack)){
						pushStack(server->sStack, letter[0]);
					}
					else{
						pushStack(server->logHist, '2');
					}
				}
				else{
					if(!isStackFull(clientsArr->arr[atoi(csNum)-1]->cStack)){
						pushStack(clientsArr->arr[atoi(csNum)-1]->cStack, letter[0]);
					}
					else{
						pushStack(clientsArr->arr[atoi(csNum)-1]->logHist, '2');
					}
				}
			}
			else if(strcmp("S", command) == 0){
				if(!isStackEmpty(clientsArr->arr[atoi(csNum)-1]->cStack)){
					char popped = popStack(clientsArr->arr[atoi(csNum)-1]->cStack);
					pushStack(clientsArr->arr[atoi(csNum)-1]->logHist, popped);
					if(!isQueueFull(server->sQueue)){
						enqueue(server->sQueue, popped);
					}
					else{
						pushStack(server->logHist, '1');
					}
				}
				else if(!isQueueEmpty(clientsArr->arr[atoi(csNum)-1]->cQueue)){
					char dequeued = dequeue(clientsArr->arr[atoi(csNum)-1]->cQueue);
					pushStack(clientsArr->arr[atoi(csNum)-1]->logHist, dequeued);
					if(!isQueueFull(server->sQueue)){
						enqueue(server->sQueue, dequeued);
					}
					else{
						pushStack(server->logHist, '1');
					}
				}
				else if(isStackEmpty(clientsArr->arr[atoi(csNum)-1]->cStack) && isQueueEmpty(clientsArr->arr[atoi(csNum)-1]->cQueue)){
					pushStack(clientsArr->arr[atoi(csNum)-1]->logHist, '3');
				}
			}
			else if(strcmp("O", command) == 0){
				if(!isStackEmpty(server->sStack)){
					char popped2 = popStack(server->sStack);
					pushStack(server->logHist, popped2);
				}
				else if(!isQueueEmpty(server->sQueue)){
					char dequeued2 = dequeue(server->sQueue);
					pushStack(server->logHist, dequeued2);
				}
				else if(isStackEmpty(server->sStack) && isQueueEmpty(server->sQueue)){
					pushStack(server->logHist, '3');
				}					
			}
		}
	}	

	int i;
	for(i=0;i<csSize-1;i++){
		displayStack(clientsArr->arr[i]->logHist, outFile);
	}
	displayStack(server->logHist, outFile);
}

int main(int argc, char* argv[]) {
	
	commandParseandWriteFile(argv[1], argv[2], argv[3]);
	
	return 0;
}
