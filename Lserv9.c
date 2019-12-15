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
#include<signal.h>
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
int USERSIZE = 0;
void client_request_processing(int);
void * handle_clnt(void *arg);

void Register() {}           //Register
void Login() {}              //Login
void show_screen() {}        //print function
void add_project() {}
void delete_project() {}
void save_bfile(FILE* fp){
printf("saving binary file start\n");
for(int i=0;i<5;i++)
fwrite(&PROJ[i],sizeof(PROJECT),1,fp);
fflush(fp);
}
void read_bfile(FILE *fp){
printf("reading binary file start\n");
for(int i=0;i<5;i++)
fread(&PROJ[i],sizeof(PROJECT),1,fp);
}
void read_USER(FILE* fp) {
	printf("reading user file start\n");
	fread(USERLIST, sizeof(USERINFO), 200, fp);
}
void save_USER(FILE* fp) {
	printf("saving user file start\n");
	fwrite(USERLIST, sizeof(USERINFO), 200, fp);
	fflush(fp);
}
void writePROJ(FILE* sock_fpo) {
	for (int i = 0; i < 5; i++) {
		printf("write the project to client\n");
		fwrite(&PROJ[i], sizeof(PROJECT), 1, sock_fpo);
	}
	fflush(sock_fpo);
}
void save_num(FILE* fp) {
	fwrite(&USERSIZE, sizeof(int), 1, fp);
	fflush(fp);
}
void read_num(FILE* fp) {
	fread(&USERSIZE, sizeof(int), 1, fp);
}

void readPROJ(FILE* sock_fpi) {
	int proj_index;
	printf("read operation start\n");
	fread(&proj_index, sizeof(int), 1, sock_fpi);
	fread(&PROJ[proj_index], sizeof(PROJECT), 1, sock_fpi);
}
void writeUSER(FILE* sock_fpo) {
	printf("write USERLIST\n");
	fwrite(&USERSIZE, sizeof(int), 1, sock_fpo);
	fflush(sock_fpo);
	for(int i=0;i<200;i++)
	fwrite(&USERLIST[i], sizeof(USERINFO), 1, sock_fpo);
	fflush(sock_fpo);
}
void readUSER(FILE* sock_fpi) {
	printf("read USER\n");
	fread(&USERLIST[USERSIZE++], sizeof(USERINFO), 1, sock_fpi);
}

int clnt_cnt =0;
int clnt_socks[256];
FILE* sock_fpo[256];
FILE* sock_fpi[256];
pthread_mutex_t mutx;
int main(int ac, char* av[]) {
	struct sockaddr_in saddr,clnt_adr;
	struct hostent* hp;
	FILE* usero, * useri, * numi, * numo, * saveo, * savei;;
	char hostname[HOSTLEN];
	char op[BUFSIZ];
	int sock_id, sock_fd;
	time_t thetime;
	char* ctime();
	int clnt_adr_sz;
	int index = 0;
	pthread_t t_id;
	
	useri = fopen("user.txt", "r");
	numi = fopen("num.txt", "r");
	savei = fopen("save.txt", "r");
	read_num(numi);
	read_USER(useri);
	read_bfile(savei);
	fclose(savei);
	fclose(useri);
	fclose(numi);
	pthread_mutex_init(&mutx,NULL);
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

	
	while(1){
	
		clnt_adr_sz=sizeof(clnt_adr);
		sock_fd = accept(sock_id, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		printf("Wow!got a call\n");
		if (sock_fd == -1)
			oops("accept");

		pthread_mutex_lock(&mutx);
		index = clnt_cnt;
		clnt_socks[clnt_cnt++]=sock_fd;
		pthread_mutex_unlock(&mutx);

		pthread_create(&t_id,NULL,handle_clnt,(void*) &index);
		//client_request_processing(sock_fd);
		pthread_detach(t_id);
		
		printf("Connected client IP:%s \n",inet_ntoa(clnt_adr.sin_addr));

		
	}
	close(sock_id);
	saveo = fopen("save.txt", "w");
	usero = fopen("user.txt", "w");
	numo = fopen("num.txt", "w");

	save_bfile(saveo);
	save_num(numo);
	save_USER(usero);
	fclose(saveo);
	fclose(usero);
	fclose(numo);


    return 0;	

}

void *handle_clnt(void * arg){
    int index=*((int*)arg);

    client_request_processing(index);
	fclose(sock_fpi[index]);
	fclose(sock_fpo[index]);
	sock_fpo[index] = NULL;
	sock_fpi[index] = NULL;
	close(clnt_socks[index]);
    return NULL;
}

void client_request_processing(int index){
	int x, y;
	char title[TL];
	char content[CL];
	int request;
	int PROJindex;
	int userindex = -1;
	

	sock_fpi[index] = fdopen(clnt_socks[index], "r"); //open FILE stream between server and client
	sock_fpo[index] = fdopen(clnt_socks[index], "w");//open FILE stream between server and client
	if (sock_fpi[index] == NULL||sock_fpo[index]==NULL)
		oops("fdopen");
	while (request!=EXIT_REQUEST) { 
		printf("Receive request from client \n");
		fread(&request, sizeof(int), 1, sock_fpi[index]);
		if (request == PROJR_REQUEST) {
			printf("Request is PROJR\n");
			readPROJ(sock_fpi[index]);
			request = PROJR_REQUEST;
			
			pthread_mutex_lock(&mutx);
			    for(int i=0;i<clnt_cnt;i++){
					if (sock_fpo[i] != NULL) {
						fwrite(&request, sizeof(int), 1, sock_fpo[i]);
						fflush(sock_fpo[i]);
						writePROJ(sock_fpo[i]);
					}
			    }
			pthread_mutex_unlock(&mutx);

		}
		else if (request == PROJW_REQUEST) {
			printf("Request is PROJW\n");
			writePROJ(sock_fpo[index]);
		}
		else if(request==EXIT_REQUEST){
			printf("Request is EXIT\n");
			fwrite(&request, sizeof(int), 1, sock_fpo[index]);
			fflush(sock_fpo[index]);
			break;
		}
		else if (request == USERR_REQUEST) {
			printf("Request is USERR\n");
			readUSER(sock_fpi[index]);
			printf("Current USERSIZE is%d\n", USERSIZE);
			request = USERR_REQUEST;
			for (int i = 0; i < clnt_cnt; i++) {
				if (sock_fpo[i] != NULL) {
					fwrite(&request, sizeof(int), 1, sock_fpo[i]);
					fflush(sock_fpo[i]);
					writeUSER(sock_fpo[i]);
				}
			}
		}
		else if (request == USERW_REQUEST) {
			printf("Request is USERW\n");
			writeUSER(sock_fpo[index]);
		}
		else if (request == CHANGE_STATUS) {
			printf("Request is change status\n");
			fread(&userindex, sizeof(int), 1, sock_fpi[index]);
			USERLIST[userindex].status = !USERLIST[userindex].status;
			printf("%s is %d status\n", USERLIST[userindex].name, USERLIST[userindex].status);
			request = USERR_REQUEST;
			for (int i = 0; i < clnt_cnt; i++) {
				if (sock_fpo[i] != NULL) {
					fwrite(&request, sizeof(int), 1, sock_fpo[i]);
					fflush(sock_fpo[i]);
					writeUSER(sock_fpo[i]);
				}
			}
		}
}

}


