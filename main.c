#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int inputHandler(char* input, char* list[10]){
    char* token;
    //char* list[10];
    char delim[2] = ";";
    token = strtok(input, delim);
    int index=0;
    list[index] = (char*)malloc(400);
    strcpy(list[index], token);
    index++;
    //printf("%s\n", token);
    token = strtok(NULL, delim);

    while(token != NULL){
        list[index] = (char*)malloc(400);
        strcpy(list[index], token);
        index++;
        //printf("%s\n", token);
        token = strtok(NULL, delim);
    }

    return index;
}

int inputHandler2(char* input, char* list[50]){
    char* token;
    char delim[2] = " ";
    token = strtok(input, delim);
    int index=0;
    list[index] = (char*)malloc(400);
    strcpy(list[index], token);
    index++;
    //printf("%s\n", token);
    token = strtok(NULL, delim);

    while(token != NULL){
        list[index] = (char*)malloc(400);
        strcpy(list[index], token);
        index++;
        //printf("%s\n", token);
        token = strtok(NULL, delim);
    }

    return index;
}
void command_handler(char* command_breakdown[50]){
    if(!strcmp(command_breakdown[0],"echo")){
        int i=1;
        while(command_breakdown[i]!=NULL){
            printf("%s", command_breakdown[i]);
            i++;
        }
    }
}

int main(){
    char *username = (char*)malloc(400);
    char *systemname = (char*)malloc(400);
    char *curr_directory = (char*)malloc(120);
    curr_directory = "~";
    getlogin_r(username, 100);
    gethostname(systemname, 100);


    while(1){
        char* commands_list[10];
        printf("<%s@%s:%s>", username, systemname, curr_directory);
        char* input = (char*)malloc(4000);
        fgets(input, 4000, stdin);
        //scanf("%s", input);
        int count = inputHandler(input, commands_list);
        free(input);
        for(int i=0;i<count;i++){
            char* command_breakdown[50] = {NULL};
            int count_args = inputHandler2(commands_list[i], command_breakdown);

            command_handler(command_breakdown);
        }        
    }
}

