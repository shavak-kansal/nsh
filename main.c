#include "utils.h"

extern strLink bgProcessList;
char *curr_directory;
char *home_directory;
char *last_directory;
history curr_history;
char *username;
char *systemname;
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
    strLinkInit(&bgProcessList);
    username = (char*)malloc(400*sizeof(char));
    systemname = (char*)malloc(400*sizeof(char));
    curr_directory = (char*)malloc(400*sizeof(char));
    home_directory = (char*)malloc(400*sizeof(char));
    last_directory = (char*)malloc(400*sizeof(char));
    getcwd(home_directory, 400);
    getcwd(curr_directory, 400);
    //strcpy(curr_directory, "~\0");
    //printf("curr : %s home :%s\n",curr_directory, home_directory);
    getlogin_r(username, 100);
    gethostname(systemname, 100);
    
    HistoryInit(&curr_history);

    HistoryReadFromFile(&curr_history);
    //printf("%d\n", getpid());
    while(1){
            prompt();
        }        
}

void prompt(){
        StringVector CommandList;
        StringVectorInit(&CommandList);
        
        char *msg1;
        msg1 = pwd();
        printf("<%s@%s:%s>", username, systemname, msg1);
        free(msg1);

        char* input = (char*)malloc(4000);
        fgets(input, 4000, stdin);
        
        //if(input[strlen(input)-1]=='\n')
        //    input[strlen(input)-1]='\0';

        input[strcspn(input, "\n")] = 0;

        InputSanitize(input, &CommandList, ";");
        free(input);

        //for(int i=0;i<CommandList.size;i++)
        //    addToHis(CommandList.list[i], &curr_history);

        for(int i=0;i<CommandList.size;i++){
            addToHis(CommandList.list[i], &curr_history);
            StringVector commandBreakdown;
            StringVectorInit(&commandBreakdown);
            InputSanitize(CommandList.list[i], &commandBreakdown, " \t");

            if(!strcmp(commandBreakdown.list[0], "history")){
                if(commandBreakdown.size==1)
                    HistoryPrint(-1, &curr_history);
                else
                    HistoryPrint(atoi(commandBreakdown.list[1]) , &curr_history);
            }
            else if(!strcmp(commandBreakdown.list[0], "exit")){
                HistoryWriteToFile(&curr_history);
                return;
            }
            else 
                CommandHandler(&commandBreakdown);
            
            StringVectorErase(&commandBreakdown);
        }
}
