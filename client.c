#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 5984
#define BUFF_SIZE 4096

int main(int argc, const char *argv[])
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[BUFF_SIZE] = {0};

	/* [C1: point 1]
	 * sock will hold the file descriptor handle returned
	 * by the call to 'socket'. As the domain we pass in AF_INET
	 * which specifies to use the IPv4 protocol for the server.
	 * We then use SOCK_STREAM for the type of socket which 
	 * specifies we want to use a TCP type connection. Lastly, we
	 * specify a protocol value of 0 so the protocol is automatically
	 * determined for us.
	 * If a less-than 0 value is returned by 'socket', it implies
	 * an error occurred, then we print an error msg and exit 
	 * the executable.
	 */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	/* [C2: point 1]
	 * In this first line with the memset call, we set each byte of memory
	 * associated with the serv_addr struct to the '0' char value which
	 * is equivalent to ASCII (int) value of 48. We then set the
	 * 'sin_family' value of the struct to represent an IPv4 connection
	 * with the AF_INET argument. Then we set the port value to the same
	 * port as our server program, where the port number is converted
	 * to be of the same bit endianness as the network.
	 */
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	/* [C3: point 1]
	 * The inet_pton function converts the given char* IP address of
	 * '127.0.0.1' to the proper address structure of type IPv4, given
	 * by the AF_INET address family argument. This converted address
	 * is a network address that is now stored in the serv_addr.sin_addr
	 * struct value. This function returns less-than-or-equal-to 0 on an
	 * error, so we exit the program if that's the case. 
	 * We should also note that the address of 127.0.0.1 is the local
	 * address, so this client will be connecting to the local address
	 * of the machine that's running it. 
	 */
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	/* [C4: point 1]
	 * This is the connect system call which connects the socket 'sock'
	 * to the address referred to by 'serv_addr' struct which we had set
	 * up in the previous steps. On error, it retuns a less than zero 
	 * resulting value, so we quit the program. If the address given is
	 * not available on the requested port, then we can get an error.
	 * This means we need to have our server running for this to work.
	 * Notice how we don't need a new socket here, once the socket 'sock'
	 * is in a connected state, we can use it.
	 */
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}


	/* [C5: point 1]
	 * In this section we ask the user to press a key on their keyboard
	 * in order to unblock the getchar() call that waits for input from
	 * stdin.  
	 * Notice that this line will not get hit until a connection has
	 * been established between server and client.
	 */
	printf("Press any key to continue...\n");
	getchar();

	/* [C6: point 1]
	 * Once the user unblocks the getchar() call, we use the send call to
	 * send the char array message in 'hello' to the server referred to
	 * by the connected socket 'sock'. With the 0 passed in for the
	 * 'flags' argument, this call is equivalent to a 'write' syscall.
	 */
	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");

	/* [C7: point 1]
	 * This call then reads from the socket 'sock' the amount of 1024
	 * bytes into the container 'buffer'. We print out the received
	 * message char array 'buffer' and then we exit the program.
	 */
	read( sock , buffer, 1024);
	printf("Message from a server: %s\n",buffer );
	return 0;
}
