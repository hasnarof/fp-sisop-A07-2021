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
#define STR_SIZE 1024
#include "_includean.c"

int socketawal = -1;
bool socketakhir = false;
const int ceksize = sizeof(int) * gas;
const int input = sizeof(int)* gas * 1;

int length_to_space(char *s) {
  char *i = s;
  while (*i != ' ' && *i != '\0') {
    i++;
  }
  return i - s;
}

char *first(char *pointer){
    char  *context;
    int inputLength = strlen(pointer);
    char *inputCopy = (char*) calloc(inputLength + 1, sizeof(char));
    strncpy(inputCopy, pointer, inputLength);

    char *firstWord = strtok_r (inputCopy, " ", &context);
	return firstWord;
    
}

char *split_space(int index, char *pointer) {
//   char *pointer = line;
  int counted = 1;
  while (*pointer != '\0') {
    if (*pointer == ' ') {
      if (counted == index) {
        int new_size = length_to_space(++pointer);
        char *word = malloc(new_size + 1);// dynamically allocate memory 
        for (int i = 0; i < new_size; i++) {
          word[i] = pointer[i];
        }
        word[new_size] = '\0';// char to end of the string '\0'
        return word;// return should be out of the loop
      }
      counted++;
    }
    pointer++;
  }
  return 0;
}

int spaces(char *str){
    int i, space=0;
    while(i<=str[i]){
        if(str[i]==' '){
            space++;
        }
        i++;
    }
    return space;
}

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
char validator[hadeh][300]; 
const int SIZE_BUFFER = sizeof(char) * 300;
void *utama(void *argv);
// void login(char *buf, int fd);
// void daftar(char *buf, int fd);
// void add(char *buf, int fd);
// void download(char *filename, int fd);
// void hapus(char *filename, int fd);
// void see(char *buf, int fd, bool isFind);
// void runninglog(char *cmd, char *filepath);
// int ambilinput(int fd, char *prompt, char *cursor);
// int validasi(int fd, char *id, char *password);
// int masukkanfile(int fd, char *dirname, char *targetFileName);
// int kirim(int fd, char *filename);
// char *ceknamafile(char *filePath);
// bool loginberhasil(FILE *fp, char *id, char *password);
bool sudahregister(FILE *fp, char *id);
// bool sudahdownload(FILE *fp, char *filename);
// void pemisahfile(char *filepath, char *namafile, char *ext);

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

void create_user(char *user, char *pass, int fd){
    FILE *fp = fopen("akun.txt", "a+");

    // if (validasi(fd, user, pass) != 0) {
        if (sudahregister(fp, user)) {
            send(fd, "Username tersebut sudah ada\n", SIZE_BUFFER, 0);
        } else {
            fprintf(fp, "%s:%s\n", user, pass);
            send(fd, "Register akun berhasil\n", SIZE_BUFFER, 0);
        }
    // }
    fclose(fp);
}

void *utama(void *argv)
{
    int fd = *(int *) argv;
    char cmd[300];
    chdir("/home/zahra/Documents/git_env/fp-sisop-A07-2021/g/zahra");

    while (recv(fd, cmd, 300, MSG_PEEK | MSG_DONTWAIT) != 0) {
        if (fd != socketawal) {
            if (ambilinput(fd, "\nCommand= ", cmd) == 0) break;
            write(fd, "\n", SIZE_BUFFER);
            
            char *kata1 = first(cmd);
            char *kata2 = split_space(1, cmd);
            if ((strcmp(kata1, "CREATE") == 0) && (strcmp(kata2, "USER") == 0)) 
            {
                free(kata2);
                free(kata1);
                char *user_name = split_space(2, cmd);
                char *pass = split_space(5,cmd);
                create_user(user_name, pass, fd);

            } 
            else if (strcmp(kata1, "CREATE") == 0 &&  strcmp(kata2, "DATABASE")==0)
            {
                free(kata2);
                free(kata1);
                char *dbName = split_space(2,cmd);
                mkdir(dbName, 0777);
                free(dbName);
                printf("Database berhasil dibuat");
                
            } 

            else if (strcmp(kata1, "CREATE") == 0 &&  strcmp(kata2, "TABLE")==0)
            {
                free(kata2);
                free(kata1);
                // char *tName = split_space(2,cmd);
                // // int spaces = spaces(cmd);
                // while(split_space(i, cmd)==NULL)
                // send(fd, "Error: Invalid command\n", SIZE_BUFFER, 0);
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


int validasi(int fd, char *id, char *password)
{
    if (ambilinput(fd, "Masukkan Username = ", id) == 0) return 0;
    if (ambilinput(fd, "Masukkan Password = ", password) == 0) return 0;
    return 1;
}

int ambilinput(int fd, char *prompt, char *cursor)
{
    send(fd, prompt, SIZE_BUFFER, 0);

    // Get input
    int count, ret_val;
    ioctl(fd, FIONREAD, &count);
    count /= 300;
    for (int i = 0; i <= count; i++) {
        ret_val = recv(fd, cursor, 300, 0);
        if (ret_val == 0) return ret_val;
    }
    while (strcmp(cursor, "") == 0) {
        ret_val = recv(fd, cursor, 300, 0);
        if (ret_val == 0) return ret_val;
    }
    printf("Command Client = %s\n", cursor);
    return ret_val;
}



//cek sudah regist apa belum
bool sudahregister(FILE *fp, char *id)
{
    char db[300], *tmp;
    while (fscanf(fp, "%s", db) != EOF) {
        tmp = strtok(db, ":");
        if (strcmp(tmp, id) == 0) return true;
    }
    return false;
}
