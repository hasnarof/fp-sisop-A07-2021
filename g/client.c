// Client side C/C++ program to demonstrate Socket programming
#include "_includean.c"

char permission[STR_SIZE]; // enum: "root", "user";

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

    char command[STR_SIZE], username[STR_SIZE], password[STR_SIZE], response[STR_SIZE];


	if (getuid()) { // not root
		strcpy(permission,"user");
		send(sock, permission, STR_SIZE, 0);

		strcpy(username, argv[2]);
		strcpy(password, argv[4]);
		send(sock, username, STR_SIZE, 0);
		send(sock, password, STR_SIZE, 0);

		memset(response, 0, sizeof(response));
		valread = read(sock, response, STR_SIZE);
		printf("%s\n", response);

		exit(0);

		if (equal(buffer, "Login success")) {
			// while (true) {
			// }
			"berhasil login";
			exit(EXIT_SUCCESS);
		}	



	} else {
		strcpy(permission, "root");
		send(sock, permission, STR_SIZE, 0);

		
	}
	

	

	

	/* ----------------------------- */
	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}
