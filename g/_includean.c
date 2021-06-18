// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>


#define PORT 8080

#define STR_SIZE 1024
#define row_size_select 20
#define column_size_select 20
#define ARRAY_SIZE 20

char file_path[] = "test.csv";
char success_message[] = "Command successfully executed.";
char fail_message[] = "There is something wrong.";
char not_allowed[] = "You don't have access.";

char login_success[] = "You are logged in.";
char login_fail[] = "Your credential not matched";

bool equal(char *s1, char *s2) {
    int i = 0, \
        d = 0;
    
    for (i = 0; s1[i] != '\0'; i++) {
        if (toupper(s1[i]) != toupper(s2[i]))
            return false;
    }
    return true;
}