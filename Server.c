#include "Server.h"
#include <stdio.h>
#include <stdlib.h>

struct Server server_constructor( int domain, int service, int protocol, u_long interface, int port, int backlog, void( *launch )( struct Server *server ) )
{
	struct Server server;
	server.domain = domain;
	server.service = service;
	server.protocol = protocol;
	server.port = port;
	server.backlog = backlog;
	int option = 1;

	server.address.sin_family = domain;
	server.address.sin_port = htons( port );
	server.address.sin_addr.s_addr = htonl( interface );

	// creating the socket
	server.socket = socket( server.domain, server.service, server.protocol );
	if( server.socket == 0 )
	{
		perror( "Failed to connect to socket...\n" );
		exit(1);
	}

	// Forcefully attach the socket
	if( setsockopt( server.socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof( option ) ) )
	{
		perror( "setsockopt\n" );
		exit(1);
	}

	// assigns the address to the socket
	if( bind( server.socket, ( struct sockaddr * )&server.address, sizeof( server.address ) ) < 0 )
	{
		perror( "Failed to bind socket...\n" );
		exit(1);
	}

	if( listen( server.socket, server.backlog ) < 0 )
	{
		perror( "Failed to start listening" );
		exit(1);
	}

	server.launch = launch;

	return server;
}