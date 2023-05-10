#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void errors(int eno, int sockfd)
{
	switch(eno)
	{
		case 1 : printf("\nSocket error!\n");
				 break;
		case 2 : printf("\nBinding error!\n");
				 break;
		case 3 : printf("\nError while listening!\n");
				 break;
		case 4 : printf("\nError while establishing connection!\n");
				 break;
		case 5 : printf("\nWrong Credentials!\n");
				 break;
	}
	printf("Exiting...\n");
	close(sockfd);
	exit(1);
}

void main()
{
	char username[20];
	char password[20];
	char message[100];
	char reverse[100];

	int server_sockfd;
	int err;
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//socket
	if(server_sockfd == -1)
	{
		errors(1, server_sockfd);
	}
	else
	{
		err = bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));//bind
		if(err == -1)
		{
			errors(2, server_sockfd);
		}
		else
		{
			err = listen(server_sockfd, 5);//listen
			if(err == -1)
			{
				errors(3, server_sockfd);
			}
			else
			{
				printf("Server is listening on port 8080...\n");
				struct sockaddr_in client_addr;
				socklen_t client_len = sizeof(client_addr);
				int client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_len);//accept
				if(client_sockfd == -1)
				{
					errors(4, server_sockfd);
				}
				else
				{
					read(client_sockfd, username, sizeof(username));
					read(client_sockfd, password, sizeof(password));
					if(strcmp(username, "anjoe") !=0 || strcmp(password, "123") != 0)
					{
						write(client_sockfd, "1", 1);
						errors(5, server_sockfd);
					}
					else
					{
						write(client_sockfd, "0", 1);
						printf("\nSuccesfully connected...\n");
						read(client_sockfd, &message, sizeof(message));//read
						printf("\nMessage recieved from the client: %s\n", message);
						printf("\nReversing the string...\n");
						int length = strlen(message);
						int i = 0;
						while(message[i] != '\0')
						{
							reverse[length - i] = message[i++];
						}
						write(client_sockfd, reverse, sizeof(reverse));//write
						printf("\nMessage successfully trasmitted.\n");
						close(server_sockfd);
					}	
				}
			}
		}
	}
}
