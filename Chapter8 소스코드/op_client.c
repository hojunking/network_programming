#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE];
	char message[BUF_SIZE];
	int opnd_cnt, i;
	struct sockaddr_in serv_adr;

    char hostname[1024];
	char result[1024];

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	while(1) 
	{
		fputs("Enter hostname (Q to quit): ", stdout);
		fgets(hostname, sizeof(hostname), stdin);
        hostname[strcspn(hostname, "\r\n")] = 0;
		//printf("%s",hostname);
		
		if (strcmp(hostname, "Q") == 0 || strcmp(hostname, "q") == 0)
            break;
		
		opmsg[0]=(char)opnd_cnt;
		
		write(sock, hostname, 1024);
		read(sock, &result, BUF_SIZE);
		printf("%s\n", result);
		
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}