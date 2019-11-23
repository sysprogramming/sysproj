#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define TL 50      //title length
#define CL 200     //Content string length
#define RL 50     //Reply length
#define DOB 0      //DO_BLOCK delimiter
#define DOINGB 1   //DOING_BLOCK delimiter
#define DONEB 2    //DONE_BLOCK delimiter
#define DOI 5
#define DOINGI 40
#define DONEI 75
#define MENUI 110
int PROJnum = 0;

void clrscr(void)
{
    write (1, "\033[1;1H\033[2J", 10);
}
void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
}


typedef struct USERINFO {
	char ID[20];
	char PW[20];
	char name[20];
} USERINFO;

typedef struct TASK_BLOCK {
	char title[TL];
	char content[CL];
	char reply[5][40];
} TASK_BLOCK;

typedef struct PROJECT {
	char title[TL];
	TASK_BLOCK DOARR[3][30];
	USERINFO USERARR[30];

	int DOSIZE;
	int DOINGSIZE;
	int DONESIZE;
}PROJECT;


int add_block(TASK_BLOCK* add) {
	gotoxy(MENUI, 11);
	printf("TITLE NAME:");
	scanf("%s", add->title);
	gotoxy(MENUI, 12);
	printf("CONTENT:");
	scanf("%s", add->content);
}
void show_block(TASK_BLOCK arr[30],int size,int printindex) {
	int length = 0;
	int yindex = 5;
	for (int i = 0; i < size; i++) {
		length = strlen(arr[i].content);
		gotoxy(printindex, yindex++);
		printf(" ------------------------------ ");
		gotoxy(printindex, yindex++);
		printf("|%-30s|", arr[i].title);
		gotoxy(printindex, yindex++);
		printf("|------------------------------|");
		for (int j = 0; j <= length / 30;j++) {
			gotoxy(printindex, yindex++);
			printf("|");
			for (int k = 0;k < 30; k++) {
				if (arr[i].content[j * 30 + k]>=33)
					printf("%c", arr[i].content[j * 30 + k]);
				else
					printf(" ");
			}
			printf("|");
		}
		gotoxy(printindex, yindex++);
		printf("|------------------------------|");
		
		for (int j = 0; strlen(arr[i].reply[j]) !=0; j++) {
			gotoxy(printindex, yindex++);
			printf("|%-30s|\n", arr[i].reply[j]);
		}
		gotoxy(printindex, yindex++);
		printf(" ------------------------------ ");
		yindex++;
		
	}
}
void delete_block() {}
void Register() {}           //Register
void Login() {}              //Login
void show_screen() {}        //print function
void add_project() {}
void delete_project() {}
void move_DOB() {}           //move DO_BLOCK to DOING_BLOCK
void move_DOINGB() {}        //move DOING_BLOCK to DONE_BLOCK

PROJECT a = { 0 };
int main() {	
	char op;
clrscr();
	while (1) {

		show_block(a.DOARR[0], a.DOSIZE, DOI);
		gotoxy(MENUI, 10);
		printf("명령어를 입력하세요:");
		scanf("%c", &op);
		if (op == 'a')
			 add_block(&a.DOARR[0][a.DOSIZE++]);
		else if (op == 'q'){
clrscr();
			break;
}
		clrscr();
	}
}
