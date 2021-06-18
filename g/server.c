#include "_includean.c"

char permission[STR_SIZE];
char auth_file_path[] = "databases/rootdatabase/auth.text";

bool login(char* username, char* password) { // return true if founded
    // Substitute the full file path
    // for the string file_path
    FILE* fp = fopen(auth_file_path, "r");
  
    if (!fp)
        printf("Can't open file\n");
  
    else {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[STR_SIZE];
  
        int row = 0;
        int column = 0;
  
        while (fgets(buffer, STR_SIZE, fp)) {
            column = 0;
  
            // To avoid printing of column
            // names in file can be changed
            // according to need
            if (row == 0) {
                continue;
            } else {
                // Splitting the data
                char* value = strtok(buffer, ", ");
    
                while (value) {
                    if(equal(value, username)) {
						value = strtok(NULL, ", ");
						column++;
						if(equal(value, password)) {
							return true;
						} else {
							return false;
						}
					} else {
						continue;
					}
                    
                }
    
            }
  
            

            row++;
        }

		return false;
  
        // Close the file
        fclose(fp);
    }
}

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
					(socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	char username[STR_SIZE], password[STR_SIZE];

	memset(permission, 0, sizeof(permission));
	valread = read( new_socket , permission, STR_SIZE); //permission

	if(equal(permission, "user")) {
		memset(username, 0, sizeof(username));
		valread = read( new_socket , username, STR_SIZE);
		memset(password, 0, sizeof(password));
		valread = read( new_socket , (char*)password, STR_SIZE);

		bool isAuthenticated = login(username, password);

		if( !isAuthenticated ) {
			send(new_socket, login_fail, STR_SIZE, 0);
		} else {
			send(new_socket, login_success, STR_SIZE, 0);
		}


	}

	exit(0);




	valread = read( new_socket , buffer, 1024);
	printf("%s\n",buffer );
	send(new_socket , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	return 0;
}
