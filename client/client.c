#include <stdio.h>
#include <string.h>
// #include <cstring>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#define cuk 0.001
#define gas 512-(212*1)

const int cekisi = sizeof(char) * gas;
const int ceksize = sizeof(int) * gas;
const int input = sizeof(int)* gas * 1;

//pembuatan socket server
int create_socket()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    int opt = 1;
    struct hostent *local_host; 
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(7000);
    local_host = gethostbyname("127.0.0.1");
    saddr.sin_addr = *((struct in_addr *)local_host->h_addr);
    ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}
char command[gas];
bool cek = false;
bool posisi = true;
bool cuek = false;

void *cekinput(void *fdc)
{
    chdir("/home/zahra/Documents/git_env/fp-sisop-A07-2021/g/zahra");
    int fd = *(int *) fdc;
    char message[gas] = {0};

    while (1) {
        gets(message);
        send(fd, message, cekisi, 0);
        if (cek) {
            strcpy(command, message);
        }
    }
}
void activeserver(int fd, char *input)
{
    if (recv(fd, input, gas, 0) == 0) {
        printf("Server telah meninggal\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char const *argv[])
{
    // pthread_t tid[2];
    int fdc = create_socket();

    close(fdc);
    return 0;
}