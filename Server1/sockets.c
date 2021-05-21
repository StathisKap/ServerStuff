#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void error(char *msg);


int main()
{
	int reuse = 1;
	int listener_d = socket(PF_INET,SOCK_STREAM,0);
	if ( listener_d == -1)
		error("Can't open socket");
	
	struct sockaddr_in name; //Creates a port
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t)htons(30000);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int c = bind (listener_d,(struct sockaddr *) &name,sizeof(name));
	if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
		 error("Can't set the reuse option on the socket");
	if ( c == -1)
		error("Can't bind to socket");

	if( listen(listener_d, 10) == -1) //creates a queue of 10 people long
		error("Can't listen");
	
	struct sockaddr_storage client_addr; //Accepts connections and creates sockets for communication with each client
	unsigned int address_size = sizeof(client_addr);
	int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
	if (connect_d == -1)
		error("Can't open secondary socket");

	char *msg = "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! Knock!\r\n> ";
	if (send(connect_d, msg, strlen(msg), 0) == -1)
 		error("send");

	return 0;
}


void error(char *msg)
{
	printf("%s: %s",msg,strerror(errno));
	exit(1);
}
