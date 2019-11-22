#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

#define TL 50      //title length
#define CL 200     //Content string length
#define RL 100     //Reply length
#define DOB 0      //DO_BLOCK delimiter
#define DOINGB 1   //DOING_BLOCK delimiter
#define DONEB 2    //DONE_BLOCK delimiter


typedef struct USERINFO{
char ID[20];
char PW[20];
char name[20];
} USERINFO;

typedef struct TASK_BLOCK{
char title[TL];
char content[CL];
char reply[RL][RL];
} TASK_BLOCK;

typedef struct PROJECT{
char title[TL];
TASK_BLOCK DOARR[3][30];
USERINFO USERARR[30];

int DOSIZE;
int DOINGSIZE;
int DONESIZE;
}PROJECT;

void add_block(TASK_BLOCK *add){

printf("TITLE NAME:");
scanf("%s",add->name);
printf("CONTENT:");
scanf("%s",add->content);
}
void delete_block(){}
void Register(){}           //Register 
void Login(){}              //Login
void show_screen(){}        //print function
void add_project(){}   
void delete_project(){}
void move_DOB(){}           //move DO_BLOCK to DOING_BLOCK
void move_DOINGB(){}        //move DOING_BLOCK to DONE_BLOCK


