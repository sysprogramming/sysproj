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
void clrscr(void)
{
	write(1, "\033[1;1H\033[2J", 10);
}

int PROJnum = 0;

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
PROJECT PROJ[5] = { 0 };

int add_block(TASK_BLOCK* add, char* title, char* content) {
	strcpy(add->title, title);
	strcpy(add->content, content);
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
void move_BLOCK(TASK_BLOCK a[30], TASK_BLOCK b[30], int y, int size1, int size2) {
	b[size2] = a[y];
	delete_block(a, y, size1);
}
int main(int ac, char* av[]) {
	struct sockaddr_in servadd;
	struct hostent* hp;
	int length;
	int sock_id, sock_fd;
	char message[10000];
	int messlen;
	char op[20];
	int x, y;
	FILE* sock_fpo;
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
	while (1) {
		show_block(PROJ[0].ARR[0], PROJ[0].SIZE[0], DOI);
		show_block(PROJ[0].ARR[1], PROJ[0].SIZE[1], DOINGI);
		show_block(PROJ[0].ARR[2], PROJ[0].SIZE[2], DONEI);
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
			add_block(&PROJ[0].ARR[0][PROJ[0].SIZE[0]++], title, content);

		}
		else if (op[0] == 'd') {
			gotoxy(MENUI, 11);
			printf("Type the position you want to delete(x y):");
			scanf("%d %d", &x, &y);

			if (x > 3 || x < 1)
				printf("Wrong position\n");
			else {
				delete_block(PROJ[0].ARR[x - 1], y - 1, PROJ[0].SIZE[x - 1]--);
			}
		}
		else if (op[0] == 'm') {
			gotoxy(MENUI, 11);
			printf("Type the position you want to move(x y):");
			scanf("%d %d", &x, &y);
			if (x > 2 || x < 1)
				printf("Wrong Position\n");
			else {
				move_BLOCK(PROJ[0].ARR[x - 1], PROJ[0].ARR[x], y - 1, PROJ[0].SIZE[x - 1]--, PROJ[0].SIZE[x]++);
			}
		}
		else if (op[0] == 'q') {
			clrscr();
			break;
		}
		clrscr();
		for (int i = 0; i < 30; i++)
			fwrite(&PROJ[0].ARR[0][i], sizeof(struct TASK_BLOCK), 1, sock_fpo);
	}
}

