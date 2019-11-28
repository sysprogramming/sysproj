#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define oops(msg) {perror(msg);exit(1);}

int main(int ac,char *av[]){
struct sockaddr_in servadd;
struct hostent *hp;
int length;
int sock_id,sock_fd;
char message[10000];
int messlen;
while(1){
sock_id=socket(AF_INET,SOCK_STREAM,0);
if(sock_id==-1)
oops("socket");

bzero(&servadd,sizeof(servadd));
hp=gethostbyname(av[1]);
if(hp==NULL)
oops(av[1]);
bcopy(hp->h_addr,(struct sockaddr *)&servadd.sin_addr,hp->h_length);
servadd.sin_port=htons(atoi(av[2]));
servadd.sin_family=AF_INET;

if(connect(sock_id,(struct sockaddr*)&servadd,sizeof(struct sockaddr))!=0)
oops("connect");
while((length=read(sock_id,message,10000))!=0){
sleep(1);
write(1,message,length);
}
}
}
