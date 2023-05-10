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
		case 2 : printf("\nError while establishing connection!\n");
				 break;
		case 3 : printf("\nWrong Credentials!\n");
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
	char status[1];
	char message[100];
	char reverse[100];

	int client_sockfd;
	int err;

	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sockfd == -1)
	{
		errors(1, client_sockfd);
	}
	else
	{
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(8080);
		server_addr.sin_addr.s_addr = INADDR_ANY;
		err = connect(client_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		if(err == -1)
		{
			errors(2, client_sockfd);
		}
		else
		{
			printf("\nConnecting to server...\n");
			printf("Enter the username: ");
			scanf("%s", username);
			printf("Enter the password: ");
			scanf("%s", password);
			write(client_sockfd, username, sizeof(username));
			write(client_sockfd, password, sizeof(password));
			read(client_sockfd, &status, sizeof(status));
			if (strcmp(status, "1") == 0)
			{
				errors(3, client_sockfd);
			}
			else
			{
				printf("\nEnter the message: ");
				scanf("%s", message);
				printf("\nSending message to server...\n");
				write(client_sockfd, message, sizeof(message));
				read(client_sockfd, &reverse, sizeof(reverse));
				printf("\nReversed string: %s\n", reverse);
				close(client_sockfd);
			}
			
		}

		
	}
}
