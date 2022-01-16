#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Server.h"

void launch( struct Server *server )
{
	char buffer[1024];
	int address_length = sizeof( server->address );
	int new_socket;

	while(1)
	{
		printf("========= WAITING ==========\n");

		new_socket = accept( server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length );

		read( new_socket, buffer, 1024 );
		printf("%s\n", buffer );

		char *hello = "Hello From Server";

		write( new_socket, hello, strlen( hello ) );
		close( new_socket );
	}
}

int main(int argc, char const *argv[])
{
	struct Server server = server_constructor( AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch );

	server.launch( &server );

	return 0;
}