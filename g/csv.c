

#include "_includean.c"


bool extraction(char* file_path, bool isPrint, char* columnList) { // return true if founded
    // Substitute the full file path
    // for the string file_path
    FILE* fp = fopen(file_path, "r");
  
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
                char buf[] ="abc/qwe/ccd";
                // int i = 0;
                char *str_colList = strtok (columnList, ",");
                int columnPosition[ARRAY_SIZE];
                int size = 0;

                while (str_colList != NULL)
                {
                    int i = 0;
                    char *str_row = strtok (buffer, ",");
                    bool found = false;

                    while(str_row != NULL && found == false) {
                        if(strcmp(str_colList, str_row) == 0) {
                            columnPosition[size++] = i;
                            found = true;

                        }

                        str_row = strtok (NULL, ",");

                    }

                    if(found == false) {
                        fprintf(stderr, "Column tidak ditemukan.\n");
                        exit(EXIT_FAILURE);
                    }
                    
                    str_colList = strtok (NULL, ",");
                }

            } else {
                // Splitting the data
                char* value = strtok(buffer, ", ");
    
                while (value) {
                    // Column 1
                    if (column == 0) {
                        printf("Name :");
                    }
    
                    // Column 2
                    if (column == 1) {
                        printf("\tAccount No. :");
                    }
    
                    // Column 3
                    if (column == 2) {
                        printf("\tAmount :");
                    }
    
                    printf("%s", value);
                    value = strtok(NULL, ", ");
                    column++;
                }
    
                printf("\n");
            }
  
            

            row++;
        }
  
        // Close the file
        fclose(fp);
    }
}

int create_database(char* dirname) {
    int check;
  
    check = mkdir(dirname,0777);
  
    // check if directory is created or not
    if (!check)
        printf("Directory created\n");
    else {
        printf("Unable to create directory\n");
        exit(1);
    }
}

// Driver Code
int main()
{
    // extraction();
    return 0;
}