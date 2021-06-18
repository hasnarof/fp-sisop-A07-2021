#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

#define SIZE 50
#define STR_SIZE 1024
#define PORT 8080
#define N_OF_CONNECTION 5

pthread_t tid[3000];
int find;
int total = 0;

bool equal(char *s1, char *s2) {
    int i = 0, \
        d = 0;
    
    for (i = 0; s1[i] != '\0'; i++) {
        if (toupper(s1[i]) != toupper(s2[i]))
            return false;
    }
    return true;
}

void *client(void *tmp) {
    FILE *fp, \
        *fp2, \
        *fp3;
    
  //  Account akun, akun2;

    char buffer[STR_SIZE] = {0};
    char hello[STR_SIZE] = "you're connected";
    char deny[STR_SIZE] = "someone else is using the system, please wait...";

    char *regist = "register";
    char *login = "login";
    char *quit = "quit";

    int valread;
    int new_socket = *(int *)tmp;

    if (total == 1) {
        send(new_socket, hello, STR_SIZE, 0);
    }
    else {
        send(new_socket, deny, STR_SIZE, 0);
    }

    while (total > 1) {
        valread = read(new_socket, buffer, STR_SIZE);
        if (total == 1) {
            send(new_socket, hello, STR_SIZE, 0);
        }
        else {
            send(new_socket, deny, STR_SIZE, 0);
        }
    }

    while (true) {
        valread = read(new_socket, buffer, STR_SIZE);

        printf("hai");

        printf("%s", buffer);

        if (equal(login, buffer)) {
            // valread = read(new_socket, akun.name, STR_SIZE);
            // valread = read(new_socket, akun.password, STR_SIZE);
            printf("nah iya");
        } else if (equal(quit, buffer)) {
            close(new_socket);
            total--;
            break;
        } else {
            printf("ga nyambung");
        }
        
    }
}

int main(int argc, char const *argv[]) {
    int server_fd, \
        new_socket, \
        valread;
    int opt = 1;

    struct sockaddr_in address;
    int addr_len = sizeof(address);

    find = 0;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, 
        SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, N_OF_CONNECTION) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    

    while(true) {
        if ((new_socket = accept(server_fd, 
            (struct sockaddr *) &address, (socklen_t*) &addr_len)) < 0) {
            fprintf(stderr, "Koneksi gagal %s\n", strerror(errno));
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Koneksi terhubung dengan port: %d\n", new_socket-3);
        pthread_create(&(tid[total]), NULL, &client, &new_socket);
        total++;

    }

    return 0;
}