#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/*
 * Item: An item that is being pushed to or popped from the stack
 *       It may be float (to be used while evaluating the postfix)
 *       It may be char* (to be used while converting infix to postfix)
 */
typedef union {
	float fData;
	char* cpData;
} Item;
/*
 *
 */
typedef struct {
    Item data;
    struct Node* prev;

} Node;



typedef struct {
	Node* top;
} Stack;
/*
 *
 */
void init(Stack *s) {
	s->top=NULL;
}
/*
 *
 */
int isEmpty(Stack *s) {
	if(s->top==NULL)
	    return 1;
	else
        return 0;
}
/*
 *
 */
Item top(Stack *s) {
	return s->top->data;
}
/*
 *
 */
Item pop(Stack *s) {
   Node* oldtop=s->top;
   s->top=s->top->prev;
   Item returned=oldtop->data;
   free(oldtop);
	return returned;
}
/*
 *
 */
void push(Stack *s, Item val) {
    Node* oldtop;
    Node* newnode=(Node*)malloc(sizeof(Node));
	if(isEmpty(s))
    {
        s->top=newnode;
        newnode->data=val;
        newnode->prev=NULL;
    }
    else
    {
        oldtop=s->top;
        s->top=newnode;
        newnode->data=val;
        newnode->prev=oldtop;
    }
}
/*
 *
 */
void destroy(Stack *s) {
    if(isEmpty(s))
    {
        return;
    }
    else
    {
        Node* currenttop = s->top;
        Node* newtop;
        while (currenttop != NULL) {
            newtop = currenttop->prev;
            free(currenttop);
            currenttop = newtop;
        }
    s->top = NULL;
    }
}
/*
 * infixToPostfix: converts an expression in infix notation to
 *					to a postfix notation (Reverse-Polish Notation)
 *					ASSUME ^*+-/ operators
 *					e.g., 2 + 3 --> 2 3 +
 *					e.g., ( -2.5 + 3.14 ) * 4 --> -2.5 3.14 + 4 *
 */
 int isOperator( char c )
 {
    if( c == '*' || c == '/' || c == '+' || c == '-' || c == '(' || c== ')' || c== '^' )
        return 1;
    return 0;
}
int priority(char c)
{
    if( c == '(' || c== ')' )
            return 1;
    else if(c == '^')
        return 4;
    else if(c == '*' || c == '/')
        return 3;
    else if(c == '+' || c == '-')
            return 2;

    return 0;
}
void infixToPostfix(char* infix, char* postfix)
{
	Stack s;
	init(&s);
	char* token=strtok(infix," ");
	while(token!=NULL)
    {
        Item current;
        current.cpData=*token;

        if(isdigit(*token))
        {
            while(*token!=NULL)
            {

                *postfix=*token;
                postfix++;
                token++;
            }
            *postfix=' ';
            postfix++;
        }
        else if(isOperator(*token))
        {
            if(*token=='-'&& *(token+1)!=NULL)
            {
                while(*token!=NULL)
                {
                    printf("[ %c ]",*token);
                *postfix=*token;
                postfix++;
                token++;
                }
            *postfix=' ';
            postfix++;
            }
            else
            {if(isEmpty(&s))
                push(&s,current);
            else
            {
                if(priority(*token)==1)
                {
                    if(*token=='(')
                        push(&s,current);
                    else
                   {
                       while(s.top->data.cpData!='(')
                    {
                        *postfix=pop(&s).cpData;
                        postfix++;
                        *postfix=' ';
                        postfix++;
                    }
                    pop(&s);
                } }
                else if(priority(*token)>priority(s.top->data.cpData))
                {
                    push(&s,current);
                }
                else if(priority(*token)<=priority(s.top->data.cpData))
                {
                    Item tobeadded=pop(&s);
                    *postfix=tobeadded.cpData;
                    push(&s,current);
                    postfix++;
                    *postfix=' ';
                    postfix++;
                }
            }
        }}
        token=strtok(NULL," ");
    }
    while(!isEmpty(&s))
    {
        *postfix=pop(&s).cpData;
         postfix++;
        *postfix=' ';
         postfix++;
    }
    destroy(&s);
    *postfix=NULL;

}
/*
 * evaluatePostfix: Evaluates an expression in postfix notation
 *					 (Reverse-Polish Notation)
 *					ASSUME single-digit operands
 */
 float myevaluation(float x, float y, char myoperator)
 {
     float intermediateResult;
     if(myoperator=='+')
         intermediateResult=(x+y);
     else if(myoperator=='*')
        intermediateResult=(x*y);
     else if(myoperator=='-')
        intermediateResult=(x-y);
    else if(myoperator=='^')
        intermediateResult=((int)x^(int)y);
    else if(myoperator=='/')
        intermediateResult=(x/y);
    return intermediateResult;

 }
float evaluatePostfix(char* postfix)
{
	float result;
	Stack s;
	init(&s);
	char* token=strtok(postfix," ");
	Item intermediate;
	int i=0;
	int flag=1;
	while(token!=NULL)
    {
        if(isdigit(*token))
        {
            float x=atof(token)*flag;
            Item new;
            new.fData=x;
            push(&s,new);
            flag=1;
        }
        else if(isOperator(*token))
        {
            if(*token=='-'&& *(token+1)!=NULL){
               flag=-1;
               token++;
                continue;
            }else{
                flag=1;
            float x=pop(&s).fData;
            float y=pop(&s).fData;
            printf("x is %f \n", x);
            printf("y is %f \n", y);
            float z=myevaluation(x,y,*token);
            intermediate.fData=z;
            push(&s,intermediate);
            }
        }
         token=strtok(NULL," ");
    }
    result=s.top->data.fData;
    destroy(&s);
	return result;
}
/*
 *
 */
void replaceNewLineBySpace(char *s) {
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}
/*
 *
 */
int main(int argc, char**argv) {
    char infixExpr[256] = "";
	char postfixExpr[256] = "";

    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);

        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);

		float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);

		printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
	}
	return 0;
}
