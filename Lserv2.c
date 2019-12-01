#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<time.h>
#include<strings.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>

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

void gotoxy(int x, int y, FILE* sockfp)
{
	fprintf(sockfp, "%c[%d;%df", 0x1B, y, x);
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

int add_block(TASK_BLOCK* add,char *title,char* content) {
	strcpy(add->title, title);
	strcpy(add->content, content);
}
void delete_block(TASK_BLOCK arr[30], int index, int size) {
	for (int i = index; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
}
void show_block(TASK_BLOCK arr[30], int size, int printindex, FILE* sockfp) {
	printf("show_block start\n");
	printf("arr[0]:%s %s\n", arr[0].title, arr[0].content);
	int length = 0;
	int yindex = 5;
	for (int i = 0; i < size; i++) {
		length = strlen(arr[i].content);
		gotoxy((printindex - 1) * 37, yindex++, sockfp);
		fprintf(sockfp, " %d %d--------------------------- ", printindex, i + 1);
		gotoxy((printindex - 1) * 37, yindex++, sockfp);
		fprintf(sockfp, "|%-30s|", arr[i].title);
		gotoxy((printindex - 1) * 37, yindex++, sockfp);
		fprintf(sockfp, "|------------------------------|");
		for (int j = 0; j <= length / 30; j++) {
			gotoxy((printindex - 1) * 37, yindex++, sockfp);
			fprintf(sockfp, "|");
			for (int k = 0; k < 30; k++) {
				if (arr[i].content[j * 30 + k] >= 33)
					fprintf(sockfp, "%c", arr[i].content[j * 30 + k]);
				else
					fprintf(sockfp, " ");
			}
			fprintf(sockfp, "|");
		}
		gotoxy((printindex - 1) * 37, yindex++, sockfp);
		fprintf(sockfp, "|------------------------------|");

		for (int j = 0; strlen(arr[i].reply[j]) != 0; j++) {
			gotoxy((printindex - 1) * 37, yindex++, sockfp);
			fprintf(sockfp, "|%-30s|\n", arr[i].reply[j]);
		}
		gotoxy((printindex - 1) * 37, yindex++, sockfp);
		if (strlen(arr[i].reply[0]) != 0)
			fprintf(sockfp, " ------------------------------ ");
		yindex++;

	}
}

void Register() {}           //Register
void Login() {}              //Login
void show_screen() {}        //print function
void add_project() {}
void delete_project() {}
void move_BLOCK(TASK_BLOCK a[30], TASK_BLOCK b[30], int y, int size1, int size2) {
	b[size2] = a[y];
	delete_block(a, y, size1);
}




int main(int ac, char* av[]) {

	struct sockaddr_in saddr;
	struct hostent* hp;
	char hostname[HOSTLEN];
	char op[BUFSIZ];
	int sock_id, sock_fd;
	FILE* sock_fpo;
	char* ctime();
	time_t thetime;
	int x, y;
	char title[TL];
	char content[CL];
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");

	bzero((void*)& saddr, sizeof(saddr));
	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void*)hp->h_addr, (void*)& saddr.sin_addr, hp->h_length);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;

	if (bind(sock_id, (struct sockaddr*) & saddr, sizeof(saddr)) != 0)
		oops("bind");
	if (listen(sock_id, 1) != 0)
		oops("listen");


	sock_fd = accept(sock_id, NULL, NULL);
	printf("Wow!got a call\n");
	if (sock_fd == -1)
		oops("accept");
	sock_fpo = fdopen(sock_fd, "w");
	if (sock_fpo == NULL)
		oops("fdopen");
	
	while (1){
		
		
		printf("get a command\n");
		read(sock_fd, op, 5);
		printf("get command complete\n");
		if (op[0]=='a') {
			printf("operation is a\n");
			read(sock_fd, title, TL);
			printf("title is %s\n", title);
			read(sock_fd, content, CL);
			printf("content is %s\n", content);
			add_block(&PROJ[0].ARR[0][PROJ[0].SIZE[0]++],title,content);
		}
		else if (op[0]=='d') {
			x = op[2] - '0';
			y = op[2] - '0';
				delete_block(PROJ[0].ARR[x - 1], y - 1, PROJ[0].SIZE[x - 1]--);
		}
		else if (op[0]=='m') {
			x = op[2] - '0';
			y = op[4] - '0';
				move_BLOCK(PROJ[0].ARR[x - 1], PROJ[0].ARR[x], y - 1, PROJ[0].SIZE[x - 1]--, PROJ[0].SIZE[x]++);
		}
		else if (op[0]=='q') {
			clrscr();
			break;
		}
		printf("show block to client\n");
		show_block(PROJ[0].ARR[0], PROJ[0].SIZE[0], DOI, sock_fpo);
		show_block(PROJ[0].ARR[1], PROJ[0].SIZE[1], DOINGI, sock_fpo);
		show_block(PROJ[0].ARR[2], PROJ[0].SIZE[2], DONEI, sock_fpo);
		fflush(sock_fpo);
}
	fclose(sock_fpo);

}

