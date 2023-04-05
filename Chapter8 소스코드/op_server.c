#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> // Add this line to include the close function

#define BUF_SIZE 1024
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	clnt_adr_sz=sizeof(clnt_adr);
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);	
	printf("new client connected....");
	while(1)
	{
		char hostname[BUF_SIZE];
        read(clnt_sock, hostname, sizeof(hostname));
        hostname[strcspn(hostname, "\r\n")] = 0;
        
        // Get the IP address list for the hostname
        struct hostent *host = gethostbyname(hostname);
		//printf("%s\n", hostname);
        if (host == NULL) {
            // If the hostname is invalid, send an error message back to the client
            char error_message[] = "gethost... error";
            send(clnt_sock, error_message, sizeof(error_message), 0);
        } else {
            // Send the first IP address in the list back to the client
            for(i=0; host->h_addr_list[i]; i++){
				printf("IP addr %d: %s \n", i+1,
					inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
				char* ip_address = inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);
            	send(clnt_sock, ip_address, strlen(ip_address)+1, 0);
			}
        }
	}
	close(clnt_sock);
	close(serv_sock);
	printf("Client disconnected....");
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}