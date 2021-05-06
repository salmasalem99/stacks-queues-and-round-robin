#include <stdio.h>
#include <stdlib.h>
#include<string.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 * Item: An item that is being enqueued to or dequeued from the queue
 *       It is a pointer to the process
 */
typedef struct {
	char *name;
	int remaining_time;
} Process, *Item;
/*
 *
 */
typedef struct Node {
	Item data;
	struct Node* next;
} Node;
/*
 *
 */
typedef struct {
	Node* front;
	Node* back;
} Queue;
/*
 *
 */
void init(Queue *q) {
	q->front=NULL;
	q->back=NULL;
}
/*
 *
 */
int isEmpty(Queue *q) {
	return (q->back == NULL)&&(q->front == NULL);
}
/*
 *
 */
Item dequeue(Queue *q) {

    if(q->back==NULL || q->front==NULL)
    {
        return NULL;
    }
    Node* oldFront=q->front;

    q->front= q->front->next;
    if(q->front==NULL)
    {
        q->back=NULL;
    }

    Item returned = oldFront->data;

    free(oldFront);
    //printf("from dequeue : item is %s \n",returned->name);
    return returned;
}

void cheatEnQueue(Queue *q,Item it){//enqueue after front
    Node* newnode=(Node*)malloc(sizeof(Node));

    newnode->data=it;
    newnode->next=q->front->next;
    q->front->next=newnode;

}

/*
 *
 */
void enqueue(Queue *q, Item val) {
	Node *newBack = malloc(sizeof(Node));
	newBack->data=val;
	newBack->next=NULL;
	if(q->front==NULL)
    {
        q->front = q->back = newBack;
    }else {
        q->back->next=newBack;
        q->back=q->back->next;
    }
}
/*
 *
 */

void destroy(Queue *q) {
	while(!isEmpty(q)){
        dequeue(q);
	}

}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
    FILE* fp = fopen(filename,"r");
    int ch=0;
    int lines=1;
    if (fp == NULL)
    {
       printf("File not found");
       fclose(fp);
       return ;
    }

    while ((ch = fgetc(fp)) != EOF)
    {
      if (ch == '\n')
      lines++;
    }
    fclose(fp);
    fp = fopen(filename,"r");
    int arriv[lines];
    char names[lines][20];
    int rt[lines];
    int i = 0;

    while (!feof(fp))
    {

        fscanf(fp,"%s ",names[i]);
        fscanf(fp,"%d %d" , &arriv[i] , &rt[i]);
        i++;
    }
    fclose(fp);
    int j=0;
    Queue q;
    init(&q);
    int counter=0;
   while(1)
    {

        Item take = (Process*)malloc(sizeof(Process));
        for(int z=0; z<lines+1;z++)
        {
            if(arriv[z]==counter)
            {
                Item it = (Process*)malloc(sizeof(Process));
                it->name=names[z];
                it->remaining_time=rt[z];
                /*int cheat=0;

                if(!isEmpty(&q)){
                        int mm;
                        for(mm=0; mm<lines ; mm++)
                        {
                        if(!strcmp(names[mm],q.front->data->name)){
                            break;
                        }
                    }
                    if(arriv[z]!=arriv[mm]){//makes the new process
                        cheatEnQueue(&q,it);
                        cheat = 1;
                    }
                }
                if(!cheat)*/
                enqueue(&q,it);
                Item t3 = (Process*)malloc(sizeof(Process));

              //  printf("enqueued :%d %s\n",q.back->data->remaining_time,q.back->data->name);
            }
        }

        /*Node* ptr= q.front;
        while(ptr!=NULL){
            printf("\n  - %s\n",ptr->data->name);
            ptr=ptr->next;
        }
        while(!isEmpty(&q)){
            Item t3 = (Process*)malloc(sizeof(Process));
            t3=dequeue(&q);
            printf("dequeued :%d %s\n",t3->remaining_time,t3->name);

        }return;
        */
        if(isEmpty(&q))
        {
            printf("idle ( %d --> %d ) \n",counter,counter+1);
        }
        else{
            take=dequeue(&q);
            take->remaining_time=take->remaining_time-1;
             printf("%s ( %d --> %d )",take->name,counter,counter+1);
             for(int z=0; z<lines ; z++)
                {
                    if(!strcmp(names[z],take->name)){
                        rt[z]=rt[z]-1;
                        break;
                    }
                }
            if(take->remaining_time!=0)
            {
                enqueue(&q,take);
                printf("\n");

            }
            else if(take->remaining_time==0)
                printf(" %s HALTS\n",take->name);




        }
        int flag=0;
        for(int z=0; z<lines ; z++)
        {
            if(rt[z]!=0){
                flag=0;
            }else
                flag = 1;

        }
        counter++;
        if(flag) break;
    }
    j=0;

    /*while(j<lines)
    {
        Item it = (Process*)malloc(sizeof(Process));
        it->name=names[j];
        it->remaining_time=rt[j];
        enqueue(&q,it);
        //printf("%s\t%d \n",q.back->data->name,q.back->data->remaining_time);
        j++;
    }
    j=0;
    Item x;
    while(j<lines)
    {
        x=dequeue(&q);
      printf("%s\t%d \n",x->name,x->remaining_time);
      j++;

    }*/



    printf("EOF");
    return;

}
/*
 *
 */

int main(int argc, char**argv) {
	if(argc!=2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	RoundRobin(argv[1]);

	return 0;
}
