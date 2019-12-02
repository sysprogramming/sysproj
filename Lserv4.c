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
#define PROJW_REQUEST 100
#define PROJR_REQUEST 101
#define EXIT_REQUEST  102


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



void Register() {}           //Register
void Login() {}              //Login
void show_screen() {}        //print function
void add_project() {}
void delete_project() {}


void writePROJ(FILE* sock_fpo) {
	for (int i = 0; i < 5; i++) {
		printf("write the project to client\n");
		fwrite(&PROJ[i], sizeof(PROJECT), 1, sock_fpo);
	}
	fflush(sock_fpo);
}

void readPROJ(FILE* sock_fpi) {
	int proj_index;
	printf("read operation start\n");
	fread(&proj_index, sizeof(int), 1, sock_fpi);
	fread(&PROJ[proj_index], sizeof(PROJECT), 1, sock_fpi);
}
int main(int ac, char* av[]) {

	struct sockaddr_in saddr;
	struct hostent* hp;
	char hostname[HOSTLEN];
	char op[BUFSIZ];
	int sock_id, sock_fd;
	FILE* sock_fpi,*sock_fpo;
	char* ctime();
	time_t thetime;
	int x, y;
	char title[TL];
	char content[CL];
	int request;

	int PROJindex;
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
	sock_fpi = fdopen(sock_fd, "r");
	sock_fpo = fdopen(sock_fd, "w");
	if (sock_fpi == NULL||sock_fpo==NULL)
		oops("fdopen");
	while (request!=EXIT_REQUEST) {
		printf("Receive request from client \n");
		fread(&request, sizeof(int), 1, sock_fpi);
		if (request == PROJR_REQUEST) {
			printf("Request is PROJR\n");
			readPROJ(sock_fpi);
		}
		else if (request == PROJW_REQUEST) {
			printf("Request is PROJW\n");
			writePROJ(sock_fpo);
		}
		else if(request==EXIT_REQUEST){
			printf("Request is EXIT\n");
			break;
		}
}
	fclose(sock_fpi);
	fclose(sock_fpo);
}

