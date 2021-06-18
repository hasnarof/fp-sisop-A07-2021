#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#define gas 512-212*1
#define hadeh 2

int socketawal = -1;
bool socketakhir = false;
const int ceksize = sizeof(int) * gas;
const int input = sizeof(int)* gas * 1;

int create_socket()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    int opt = 1;
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
    saddr.sin_addr.s_addr = INADDR_ANY;
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) { 
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }
    return fd;
}

void *utama(void *argv)
{
    int fd = *(int *) argv;
    char cmd[300];
    chdir("/home/fitraharie/soal1/Server");

    while (recv(fd, cmd, 300, MSG_PEEK | MSG_DONTWAIT) != 0) {
        if (fd != socketawal) {
            if (ambilinput(fd, "\nSelamat datang di Aplikasi ini silahkan memilih command berikut(login/register)\nJawaban = ", cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);
            if (strcmp(cmd, "login") == 0) 
            {
                login(cmd, fd);
            } 
            else if (strcmp(cmd, "register") == 0) 
            {
                daftar(cmd, fd);
            } 
            else 
            {
                send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
            }
        } else 
        { 
            char prompt[300];
            strcpy(prompt, "\nSilahkan memilih perintah yang diinginkan(add,downlod,delete,see,find)\nJawaban = ");
            if (ambilinput(fd, prompt, cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);
            if (strcmp(cmd, "add") == 0) 
            {
                add(cmd, fd);
            } 
            else if (strcmp(cmd, "see") == 0) 
            {
                see(cmd, fd, false);
            }
            else 
            {
                //cekking 
                char *tmp = strtok(cmd, " ");
                char *tmp2 = strtok(NULL, " ");
                if (!tmp2) {
                    send(fd, "Perintah yang ditulis tidak ada\n", SIZE_BUFFER, 0);
                } 
                else if (strcasecmp(tmp, "download") == 0) {
                    download(tmp2, fd);
                } 
                else if (strcasecmp(tmp, "delete") == 0) {
                    hapus(tmp2, fd);
                }
                else if (strcasecmp(tmp, "find") == 0) {
                    see(tmp2, fd, true);
                }
                else {
                    send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
                }
            }
        }
        sleep(0.001);
    }
    if (fd == socketawal) 
    {
        socketawal = -1;
    }
    printf("Koneksi telah terputus diport: %d\n", fd-3);
    close(fd);
}

int main(int argc ,char const *argv1[])
{
    socklen_t addrlen;
    struct sockaddr_in new_addr;
    pthread_t tid;
    char buf[300];
    char argv[300 + hadeh];
    int new_fd, ret_val;
    int server_fd = create_socket();

    while (1) {
        new_fd = accept(server_fd, (struct sockaddr *)&new_addr, &addrlen);
        if (new_fd >= 0) {
            printf("Koneksi terhubung dengan port: %d\n", new_fd-3);
            pthread_create(&tid, NULL, &utama, (void *) &new_fd);
        } else {
            fprintf(stderr, "Koneksi gagal %s\n", strerror(errno));
        }
    }
    return 0;
}