#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define TL 51
#define CL 201
#define RL 41
#define DOB 0
#define DOINGB 1
#define DONEB 2
#define DOI 1
#define DOINGI 2
#define DONEI 3
#define MENUI 120
#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}
#define PROJI 10
#define PROJW_REQUEST 100
#define PROJR_REQUEST 101
#define EXIT_REQUEST  102

void clrscr(void)
{
	write(1, "\033[1;1H\033[2J", 10);
}

int PROJnum = 0;
int PROJindex = -1;
void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
}

void add_PROJECT(int index) {
	char buf[TL];
	gotoxy(PROJ,I2);
	printf("PROJECT TITLE");
	fgets(buf, TL - 1, stdin);
	buf[strlen(buf) - 1] = '\0';
	strcpy(PROJ[index].title, buf);
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
PROJECT PROJ[5] = { 0 };

int add_block(TASK_BLOCK* add, char* title, char* content) {  // add to-do block to project 
	strcpy(add->title, title);
	strcpy(add->content, content);
}
void delete_block(TASK_BLOCK arr[30], int index, int size) { 
	for (int i = index; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
}
void show_block(TASK_BLOCK arr[30], int size, int printindex) {  //show existing task block in current project
	int length = 0;
	int yindex = 5;
	for (int i = 0; i < size; i++) {
		length = strlen(arr[i].content);
		gotoxy((printindex - 1) * 37, yindex++);
		printf( " %d %d--------------------------- ", printindex, i + 1);
		gotoxy((printindex - 1) * 37, yindex++);
		printf( "|%-30s|", arr[i].title);
		gotoxy((printindex - 1) * 37, yindex++);
		printf( "|------------------------------|");
		for (int j = 0; j <= length / 30; j++) {
			gotoxy((printindex - 1) * 37, yindex++);
			printf( "|");
			for (int k = 0; k < 30; k++) {
				if (arr[i].content[j * 30 + k] >= 33)
					printf( "%c", arr[i].content[j * 30 + k]);
				else
					printf( " ");
			}
		printf( "|");
		}
		gotoxy((printindex - 1) * 37, yindex++);
		printf( "|------------------------------|");

		for (int j = 0; strlen(arr[i].reply[j]) != 0; j++) {
			gotoxy((printindex - 1) * 37, yindex++);
			printf( "|%-30s|\n", arr[i].reply[j]);
		}
		gotoxy((printindex - 1) * 37, yindex++);
		if (strlen(arr[i].reply[0]) != 0)
			printf(" ------------------------------ ");
		yindex++;

	}
}
void move_BLOCK(TASK_BLOCK a[30], TASK_BLOCK b[30], int y, int size1, int size2) { //move TASK_BLOCK to next state 
	b[size2] = a[y];
	delete_block(a, y, size1);
}
void show_project() { //project menu printing function
	int yindex = 5;
	for (int i = 0; i < 5; i++) {
		gotoxy(PROJI, yindex++);
		printf("----------------------------------------------------------------------------------------------");
		gotoxy(PROJI, yindex++);
		printf("I Project1: %50s I", PROJ[i].title);
		gotoxy(PROJI, yindex++);
		printf("I USERLIST: %50s I", "");
		gotoxy(MENUI + 12, yindex);
		gotoxy(PROJI, yindex++);
		printf("----------------------------------------------------------------------------------------------");
		yindex += 2;
	}
}
void readPROJ(FILE* sock_fpi) { //receive project data from server
	for (int i = 0; i < 5; i++) {
		fread(&PROJ[i], sizeof(PROJECT), 1, sock_fpi);
	}
	printf("read operation complete\n");
}
void writePROJ(FILE* sock_fpo,int PROJindex) {  // give the project data to server
	fwrite(&PROJ[PROJindex], sizeof(PROJECT), 1, sock_fpo);
	fflush(sock_fpo);
}
int main(int ac, char* av[]) {
	struct sockaddr_in servadd;
	struct hostent* hp;
	USERINFO me = { 0 };
	int length;
	int sock_id, sock_fd;
	char message[10000];
	int messlen;
	char op[20];
	int x, y;
	FILE* sock_fpo,*sock_fpi;
	int request;
	char title[TL];
	char content[CL];
	sock_id = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");

	bzero(&servadd, sizeof(servadd));
	hp = gethostbyname(av[1]);
	if (hp == NULL)
		oops(av[1]);
	bcopy(hp->h_addr, (struct sockaddr*) & servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(atoi(av[2]));
	servadd.sin_family = AF_INET;

	if (connect(sock_id, (struct sockaddr*) & servadd, sizeof(struct sockaddr)) != 0)
		oops("connect");
	clrscr();
	sock_fpo=fdopen(sock_id, "w");
	sock_fpi = fdopen(sock_id, "r");
	if (sock_fpo == NULL || sock_fpi == NULL)
		oops("FILE open");

	request = PROJW_REQUEST;
	printf("sending request\n");
	fwrite(&request, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
	printf("reading PROJECT start\n");
	readPROJ(sock_fpi);

	while(1){

		while (1) {} // show Login menu 
		while (1) { //show existing PROJECT
			
		show_project();
		gotoxy(PROJI, 1);
		printf("SELECT THE PROJECT!!! (if you want to quit, type 6 ");   //receive project index from users, 
		
			scanf("%d", &PROJindex);
			gotoxy(PROJI, 2);
			if (PROJindex < 1 && PROJindex>6) {
				printf("Wrong position! type again");
				clrscr();
				sleep(1);
			}
			else
				break;
		}
		if (PROJindex == 6) {
			request = EXIT_REQUEST;
			fwrite(&request, sizeof(int), 1, sock_fpo);
fflush(sock_fpo);
clrscr();
			break;
		}
		clrscr();
		while (1) {  //show the project content and modify the data
			show_block(PROJ[PROJindex].ARR[0], PROJ[PROJindex].SIZE[0], DOI);
			show_block(PROJ[PROJindex].ARR[1], PROJ[PROJindex].SIZE[1], DOINGI);
			show_block(PROJ[PROJindex].ARR[2], PROJ[PROJindex].SIZE[2], DONEI);

			gotoxy(MENUI, 10);
			printf("Select the operation:");
			fgets(op, 19, stdin);

			if (op[0] == 'a') {

				gotoxy(MENUI, 12);
				printf("TITLE:");
				fgets(title, TL, stdin);
				gotoxy(MENUI, 13);
				printf("CONTENT:");
				fgets(content, CL, stdin);

				title[strlen(title) - 1] = '\0';
				content[strlen(content) - 1] = '\0';
				add_block(&PROJ[PROJindex].ARR[0][PROJ[PROJindex].SIZE[0]++], title, content);
				request = PROJR_REQUEST;
				fwrite(&request, sizeof(int), 1, sock_fpo);
				fwrite(&PROJindex, sizeof(int), 1, sock_fpo);
				writePROJ(sock_fpo, PROJindex);
			}
			else if (op[0] == 'd') {
				gotoxy(MENUI, 11);
				printf("Type the position you want to delete(x y):");
				scanf("%d %d", &x, &y);

				if (x > 3 || x < 1)
					printf("Wrong position\n");
				else {
					delete_block(PROJ[PROJindex].ARR[x - 1], y - 1, PROJ[PROJindex].SIZE[x - 1]--);
					request = PROJR_REQUEST;
					fwrite(&request, sizeof(int), 1, sock_fpo);
					fwrite(&PROJindex, sizeof(int), 1, sock_fpo);
					writePROJ(sock_fpo, PROJindex);
				}
			}
			else if (op[0] == 'm') {
				gotoxy(MENUI, 11);
				printf("Type the position you want to move(x y):");
				scanf("%d %d", &x, &y);
				if (x > 2 || x < 1)
					printf("Wrong Position\n");
				else {
					move_BLOCK(PROJ[PROJindex].ARR[x - 1], PROJ[PROJindex].ARR[x], y - 1, PROJ[PROJindex].SIZE[x - 1]--, PROJ[PROJindex].SIZE[x]++);
					request = PROJR_REQUEST;
					fwrite(&request, sizeof(int), 1, sock_fpo);
					fwrite(&PROJindex, sizeof(int), 1, sock_fpo);
					writePROJ(sock_fpo, PROJindex);
				}
			}
			else if (op[0] == 'q') {
				clrscr();
				break;
			}
			clrscr();
			}
		}
		fclose(sock_fpo);
		fclose(sock_fpi);
}

