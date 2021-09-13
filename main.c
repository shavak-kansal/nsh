#include "utils.h"

int cdHandler(char* command_breakdown[50]){

    if(!strcmp(curr_directory, "~"))
        strcpy(curr_directory, home_directory);
    
    if(!strcmp(command_breakdown[1],".")){
    }
    else if(!strcmp(command_breakdown[1],"..")){
        int index = -1;
        int length = strlen(curr_directory);

        for(int i=0;i<length;i++){
            if(curr_directory[i]=='/')
                index = i;
        }
        if(index!=-1)
            curr_directory[index] = '\0';
    }
    else 
        curr_directory = command_breakdown[1];

    if(!strcmp(home_directory, curr_directory)){
        curr_directory[0]='~';
        curr_directory[1]='\0';
    }

}

int InputSanitize(char* input, StringVector* l, char* delim){
    char* token;
    
    token = strtok(input, delim);
    StringVectorAdd(l, token);
    token = strtok(NULL, delim);

    while(token != NULL){
        StringVectorAdd(l, token);
        token = strtok(NULL, delim);
    }

    return l->size;
}

int main(){
    char *username = (char*)malloc(400);
    char *systemname = (char*)malloc(400);
    curr_directory = (char*)malloc(400);
    home_directory = (char*)malloc(400);
    
    getcwd(home_directory, 400);
    strcpy(curr_directory, "~\0");
    
    getlogin_r(username, 100);
    gethostname(systemname, 100);
    
    printf("%d", getpid());
    while(1){
        StringVector CommandList;
        StringVectorInit(&CommandList);
        
        printf("<%s@%s:%s>", username, systemname, curr_directory);
        
        char* input = (char*)malloc(4000);
        fgets(input, 4000, stdin);
        
        if(input[strlen(input)-1]=='\n')
            input[strlen(input)-1]='\0';

        InputSanitize(input, &CommandList, ";");
        free(input);

        for(int i=0;i<CommandList.size;i++){
            StringVector commandBreakdown;
            StringVectorInit(&commandBreakdown);
            InputSanitize(CommandList.list[i], &commandBreakdown, " \t");

            CommandHandler(&commandBreakdown);
            StringVectorErase(&commandBreakdown);
        }        
    }
}

