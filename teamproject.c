#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define TL 51      //title length
#define CL 201     //Content string length
#define RL 41     //Reply length
#define DOB 0      //DO_BLOCK delimiter
#define DOINGB 1   //DOING_BLOCK delimiter
#define DONEB 2    //DONE_BLOCK delimiter
#define DOI 1
#define DOINGI 2
#define DONEI 3
#define MENUI 120
int PROJnum = 0;

void clrscr(void)
{
	write(1, "\033[1;1H\033[2J", 10);
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
	TASK_BLOCK ARR[3][30];
	USERINFO USERARR[30];

	int SIZE[3];
	
	
}PROJECT;


int add_block(TASK_BLOCK* add) {
	gotoxy(MENUI, 12);
	printf("TITLE NAME(max 50 chars):");
	fgets(add->title, TL, stdin);
	add->title[strlen(add->title) - 1] = '\0';
	gotoxy(MENUI, 13);
	printf("CONTENT(max 200 chars)  :");
	fgets(add->content, CL, stdin);
	add->content[strlen(add->content) - 1] = '\0';
}
void delete_block(TASK_BLOCK arr[30], int index, int size) {
	for (int i = index; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
}
void show_block(TASK_BLOCK arr[30], int size, int printindex) {
	int length = 0;
	int yindex = 5;
	for (int i = 0; i < size; i++) {
		length = strlen(arr[i].content);
		gotoxy((printindex-1)*37, yindex++);
		printf(" %d %d--------------------------- ",printindex, i+1);
		gotoxy((printindex - 1) * 37, yindex++);
		printf("|%-30s|", arr[i].title);
		gotoxy((printindex - 1) * 37, yindex++);
		printf("|------------------------------|");
		for (int j = 0; j <= length / 30; j++) {
			gotoxy((printindex - 1) * 37, yindex++);
			printf("|");
			for (int k = 0; k < 30; k++) {
				if (arr[i].content[j * 30 + k] >= 33)
					printf("%c", arr[i].content[j * 30 + k]);
				else
					printf(" ");
			}
			printf("|");
		}
		gotoxy((printindex - 1) * 37, yindex++);
		printf("|------------------------------|");

		for (int j = 0; strlen(arr[i].reply[j]) != 0; j++) {
			gotoxy((printindex - 1) * 37, yindex++);
			printf("|%-30s|\n", arr[i].reply[j]);
		}
		gotoxy((printindex - 1) * 37, yindex++);
		if (strlen(arr[i].reply[0]) != 0)
			printf(" ------------------------------ ");
		yindex++;

	}
}

void Register() {}           //Register
void Login() {}              //Login
void show_screen() {}        //print function
void add_project() {}
void delete_project() {}
void move_BLOCK(TASK_BLOCK a[30],TASK_BLOCK b[30],int y,int size1,int size2) {
	b[size2] = a[y];
	delete_block(a, y, size1);
}          
     

