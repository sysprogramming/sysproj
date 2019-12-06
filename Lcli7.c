#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

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
#define LOGIX 100
#define LOGIY 30
#define PROJW_REQUEST 100
#define PROJR_REQUEST 101
#define EXIT_REQUEST  102
#define USERR_REQUEST 103
#define USERW_REQUEST 104
#define CHANGE_STATUS 105
#define IDL 21
#define PWL 21
#define NAMEL 21
#define ONLINE 1
#define OFFLINE 0
#define LOGIN_REQUEST 200
#define REGISTER_REQUEST 300

pthread_mutex_t PROJ_lock = PTHREAD_MUTEX_INITIALIZER;

void clrscr(void)
{
	write(1, "\033[1;1H\033[2J", 10);
}

int PROJnum = 0;
int PROJindex = -1;
int USERSIZE=0;
void gotoxy(int x, int y)
{
	printf("%c[%d;%df", 0x1B, y, x);
}


typedef struct USERINFO {
	char ID[IDL];
	char PW[PWL];
	char name[NAMEL];
	int status;
} USERINFO;

typedef struct TASK_BLOCK {
	char title[TL];
	char content[CL];
	char reply[5][40];
} TASK_BLOCK;

typedef struct PROJECT {
	char title[TL];
	TASK_BLOCK ARR[3][30];
	int SIZE[3];
}PROJECT;
PROJECT PROJ[5] = { 0 };
USERINFO USERLIST[200] = { 0 };

int add_block(PROJECT *PROJ, char* title, char* content) {
	strcpy(PROJ->ARR[0][PROJ->SIZE[0]].title, title);
	strcpy(PROJ->ARR[0][PROJ->SIZE[0]++].content, content);
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
		gotoxy((printindex - 1) * 37, 1);
		if (printindex == DOI)
			printf("TO - DO things");
		else if (printindex == DOINGI)
			printf("DOING things");
		else
			printf("DONE things");
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
void show_project() {
	int yindex = 5;
	for (int i = 0; i < 5; i++) {
		gotoxy(PROJI, yindex++);
		printf("----------------------------------------------------------------------------------------------");
		gotoxy(PROJI, yindex++);
		printf("I Project %d : %50s I",i+1, PROJ[i].title);
		gotoxy(PROJI, yindex++);
		printf("I USERLIST: %50s I", "");
		gotoxy(MENUI + 12, yindex);
		gotoxy(PROJI, yindex++);
		printf("----------------------------------------------------------------------------------------------");
		yindex += 2;
	}
}
void readPROJ(FILE* sock_fpi) {
	for (int i = 0; i < 5; i++) {
		fread(&PROJ[i], sizeof(PROJECT), 1, sock_fpi);
	}
}
void writePROJ(FILE* sock_fpo,int PROJindex) {
	fwrite(&PROJ[PROJindex], sizeof(PROJECT), 1, sock_fpo);
	fflush(sock_fpo);
}
void Register(USERINFO* buf){
	int yindex;
	yindex = LOGIY;
	gotoxy(LOGIX, yindex);
	yindex += 5;
	printf("Welcome to LiRello !!!  Register new account");
	gotoxy(LOGIX, yindex++);
	printf("  ID: ");
	gotoxy(LOGIX, yindex++);
	printf("  PW: ");
	gotoxy(LOGIX, yindex);
	printf("NAME: ");
	yindex -= 2;
	gotoxy(LOGIX + 6, yindex++);
	scanf("%s", buf->ID);
	gotoxy(LOGIX + 6, yindex++);
	scanf("%s", buf->PW);
	gotoxy(LOGIX + 6, yindex);
	scanf("%s", buf->name);
}
void send_USER(FILE* fp, USERINFO buf) {
	int request = USERR_REQUEST;
	fwrite(&request, sizeof(int), 1, fp);
	fflush(fp);
	fwrite(&buf, sizeof(USERINFO), 1, fp);
	fflush(fp);
}
void read_USER(FILE* sock_fpo,FILE* sock_fpi) {
	int request = USERW_REQUEST;
	fwrite(&request, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
	for(int i=0;i<200;i++)
	fread(&USERLIST[i], sizeof(USERINFO), 1, sock_fpi);
}
void changestatus(FILE* sock_fpo, int index) {
	int request = CHANGE_STATUS;
	fwrite(&request, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
	fwrite(&index, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
}
void Login(int *USERINDEX) {
	char ID[IDL];
	char PW[PWL];
	int yindex;
	while (1) {
		yindex = LOGIY;
		gotoxy(LOGIX, yindex);
		yindex += 5;
		printf("Welcome to LiRello !!! Type your ID and PASSWORD");
		gotoxy(LOGIX, yindex++);
		printf("ID: ");
		gotoxy(LOGIX, yindex--);
		printf("PW: ");
		gotoxy(LOGIX + 4, yindex++);
		scanf("%s", ID);
		gotoxy(LOGIX + 4, yindex++);
		scanf("%s", PW);
		for (int i = 0; i <200; i++) {
			if (strcmp(USERLIST[i].ID, ID) == 0)
				if (strcmp(USERLIST[i].PW, PW) == 0) {
					*USERINDEX = i;
					return;
				}
		}
		gotoxy(LOGIX, yindex);
		printf("ID or PASSWORD is wrong !!!");
		sleep(1);
		clrscr();
	}
}
int main(int ac, char* av[]) {
	void* Reading_data(void*);
	pthread_t Rth;
	struct sockaddr_in servadd;
	struct hostent* hp;
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
	int userindex = -1;
	USERINFO userbuf;
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
	fwrite(&request, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
	readPROJ(sock_fpi);
	read_USER(sock_fpo, sock_fpi);
	
	pthread_create(&Rth,NULL,Reading_data,(void*)sock_fpi);
	while(1){
		while (userindex == -1) {
			gotoxy(LOGIX, LOGIY);
			printf("WELCOME TO LIRELLO!!!");
			gotoxy(LOGIX, LOGIY + 1);
			printf("if you want to Login, type 200, or 300 to Register");
			gotoxy(LOGIX, LOGIY + 2);
			scanf("%d", &request);
			if (request == LOGIN_REQUEST) {
				Login(&userindex);
				changestatus(sock_fpo, userindex);
			}
			else if (request == REGISTER_REQUEST) {
				Register(&userbuf);
				send_USER(sock_fpo, userbuf);
			}
			clrscr();
		}

		while (1) {
			
		show_project();
		gotoxy(PROJI, 1);
		printf("SELECT THE PROJECT!!! (if you want to quit, type 6 ");
		
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
			changestatus(sock_fpo, userindex);
			request = EXIT_REQUEST;
			fwrite(&request, sizeof(int), 1, sock_fpo);
fflush(sock_fpo);

clrscr();
			break;
		}
		clrscr();
		PROJindex -= 1;
		while (1) {
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
				add_block(&PROJ[PROJindex], title, content);
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
		
	pthread_join(Rth, NULL);
		fclose(sock_fpo);
		fclose(sock_fpi);
}

void* Reading_data(void* fp) {
	FILE* sock_fpi = (FILE*)fp;
	int request = 0;

	while (request!=EXIT_REQUEST) {
		fread(&request, sizeof(int), 1, sock_fpi);
		if (request == PROJR_REQUEST) {
			pthread_mutex_lock(&PROJ_lock);
			readPROJ(sock_fpi);
			pthread_mutex_unlock(&PROJ_lock);
		}
		else if (request == EXIT_REQUEST)
			break;
		else if (request == USERR_REQUEST) {
			pthread_mutex_lock(&PROJ_lock);
			fread(USERLIST, sizeof(USERINFO), 200, sock_fpi);
			pthread_mutex_unlock(&PROJ_lock);
		}
	}

}

void* show_ONLINEUSER(void* fp) {

}



