#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFF_SIZE] = {0};
	char *hello = "Hello from server";

	/* [S1: point 1]
	 * server_fd will hold the file descriptor handle returned
	 * by the call to 'socket'. As the domain we pass in AF_INET
	 * which specifies to use the IPv4 protocol for the server.
	 * We then use SOCK_STREAM for the type of socket which 
	 * specifies we want to use a TCP type connection. Lastly, we
	 * specify a protocol value of 0 so the protocol is automatically
	 * determined for us.
	 * If a value of 0 is returned by 'socket', then we 
	 * print an error msg and exit the executable.
	 */
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	/* [S2: point 1]
	 * The call to setsockopt sets the options for the socket
	 * referenced by server_fd. The 'level' is SOL_SOCKET which 
	 * tells the function that we want to set our own custom __optname
	 * code (argument #3). The custom operations we want to use are
	 * defined in this third argument. By using a logical OR ('|'), we
	 * are indicating to use both options. We use SO_REUSEADDR to allow
	 * re-use of local IP addresses for the socket. We use SO_REUSEPORT
	 * to allow for multiple servers to use the port for sending/receiving
	 * data. 
	 * Lastly, we pass the address of our 'opt' variable so that
	 * it can be populated with the value of the options we requested.
	 * This returns a nonzero value on error, so we check the return
	 * and fail/exit if we don't get a zero value to indicate success.
	 */
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		       &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	/* [S3: point 1]
	 * Here we set the sockaddr_in struct values. This struct holds 
	 * information about our connection. 'sin_family' is the type of
	 * connection we're making to be of the IPv4 protocol. The 
	 * 'sin_addr.s_addr' is set to INADDR_ANY which means we don't want
	 * to bind the socket to any specific IP address. Lastly, 'sin_port'
	 * is set to the result of htons(PORT) which just converts the
	 * unsigned integer PORT=5984 to be of the correct byte order for
	 * the network, such that the port to use is stored correctly.
	 */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	/* [S4: point 1]
	 * With this call we bind the server to the requested IP by passing
	 * the server file descriptor and the socket address information. It 
	 * is by design that a struct sockaddr_in can be cast to a struct 
	 * scokaddr seamlessly. If a nonzero value is returned, it implies
	 * an error occurred, so we bail from the program.
	 */
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/* [S5: point 1]
	 * This call marks the server file descriptor as being a passive
	 * socket used to accept incoming requests using the accept call.
	 * The value of '3' is given for the backlog argument which means
	 * that the queue of pending connections for the file descriptor can
	 * not exceed 3 pending connections. If the queue is full, the request
	 * is denied. 
	 * If a non-zero value is returned, it implies an error occurred, so
	 * we bail from the program. 
	 */
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* [S6: point 1]
	 * This call takes the first pending connection from the queue of
	 * connections to this socket and creates a new connected socket.
	 * This new socket is returned. We also have the address struct
	 * filled in with the peer connection address information. 
	 * This function returns a result less than zero on error and
	 * thus we exit the program in that case.
	 * Notice that this call will block until a connection request
	 * is received.
	 */
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
				 (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	/* [S7: point 1]
	 * This prints a message that asks the user to press a key. The
	 * getchar call causes this program to block until the user has
	 * entered some character.
	 * Notice that this line will not be hit until a connection is
	 * established between server and client. 
	 */
	printf("Press any key to continue...\n");
	getchar();

	/* [S8: point 1]
	 * After receiving character input from the user, the program 
	 * continues and issues the read system call to read from the 
	 * socket file descriptor. It stores 1024 bytes into the char
	 * array called 'buffer'. We then print the message in the 
	 * array to the console. 
	 */
	read( new_socket , buffer, 1024);
	printf("Message from a client: %s\n",buffer );

	/* [S9: point 1]
	 * Lastly, this code send the message stored in the char array
	 * 'hello' on the connected socket called 'new_socket'. It sends
	 * the string with the 'flags' argument of 0 which makes the send
	 * call equivalent to a 'write()' system call.
	 */
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	return 0;
}
