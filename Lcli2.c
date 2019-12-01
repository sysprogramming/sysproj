#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define MENUI 120
#define oops(msg) {perror(msg);exit(1);}
#define TL 51
#define CL 201
void gotoxy(int x, int y)
{
	printf( "%c[%d;%df", 0x1B, y, x);
}
void clrscr(void)
{
	write(1, "\033[1;1H\033[2J", 10);
}
int main(int ac, char* av[]) {
	struct sockaddr_in servadd;
	struct hostent* hp;
	int length;
	int sock_id, sock_fd;
	char message[10000];
	int messlen;
	char op[BUFSIZ];
	int x, y;
	FILE* sock_fpi;
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
		while (1) {
			gotoxy(MENUI, 10);
			printf("Select the operation:");
			fgets(op, BUFSIZ, stdin);
			if (strcmp(op, "a\n") == 0) {
				
				gotoxy(MENUI, 12);
				printf("TITLE:");
				fgets(title, TL, stdin);
				gotoxy(MENUI, 13);
				printf("CONTENT:");
				fgets(content, CL, stdin);
				title[strlen(title) - 1] = '\0';
				content[strlen(content) - 1] = '\0';
				write(sock_id, "a",5);
				sleep(1);
				write(sock_id, title, TL);
				sleep(1);
				write(sock_id, content, CL);
			
			}
			else if (strcmp(op, "d\n") == 0) {
				gotoxy(MENUI, 11);
				printf("Type the position you want to delete(x y):");
				scanf("%d %d", &x, &y);
				
				if (x > 3 || x < 1) 
					write(sock_id, "d", 5);
				else {
					sprintf(op, "d %d %d", x, y);
					write(sock_id,op,strlen(op));
				}
			}
			else if (strcmp(op, "m\n") == 0) {
				gotoxy(MENUI, 11);
				printf("Type the position you want to move(x y):");
				scanf("%d %d", &x, &y);
				if (x > 2 || x < 1)
					write(sock_id, "m",5);
				else {
					sprintf(op, "m %d %d", x, y);
					write(sock_id, op, strlen(op));
				}
			}
			else if (strcmp(op, "q\n") == 0)
				write(sock_id, "q",5);
			printf("reading message from server\n");
			length=read(sock_id, message, 10000);
				write(1, message, length);
		}
}

