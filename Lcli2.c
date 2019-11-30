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
	FILE* sock_fp;
	char title[TL];
	char content[CL];
	while (1) {
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
		sock_fp = fdopen(sock_id, "rw");
		clrscr();
		while (1) {
			gotoxy(MENUI, 10);
			printf("Select the operation:");
			fgets(op, BUFSIZ, stdin);
			if (strcmp(op, "a\n") == 0) {
				
				gotoxy(MENUI, 12);
				printf("TITLE:");
				fgets(title, TL, stdin);
				title[strlen(title) - 1] = '\0';
				gotoxy(MENUI, 13);
				printf("CONTENT:");
				fgets(content, CL, stdin);
				content[strlen(content) - 1] = '\0';
				fprintf(sock_fp, "a 0 0 %s %s", title, content);
			}
			else if (strcmp(op, "d\n") == 0) {
				gotoxy(MENUI, 11);
				printf("Type the position you want to delete(x y):");
				scanf("%d %d", &x, &y);
				if (x > 3 || x < 1)
					fprintf(sock_fp, "d 0 0 title content");
				else
					fprintf(sock_fp, "d %d %d title content", x, y);
			}
			else if (strcmp(op, "m\n") == 0) {
				gotoxy(MENUI, 11);
				printf("Type the position you want to move(x y):");
				scanf("%d %d", &x, &y);
				if (x > 2 || x < 1)
					fprintf(sock_fp, "m 0 0 title content");
				else
					fprintf(sock_fp, "m %d %d title content", x, y);
			}
			else if (strcmp(op, "q\n") == 0)
				fprintf(sock_fp, "q 0 0 title content");
			printf("reading message from server\n");
			while ((length = read(sock_id, message, 10000)) != 0) {
				sleep(1);
				write(1, message, length);
			}
		}
		
	}
}

