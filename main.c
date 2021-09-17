#include "utils.h"

//strLink bgProcessList;

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
    char *username = (char*)malloc(400);
    char *systemname = (char*)malloc(400);
    curr_directory = (char*)malloc(400);
    home_directory = (char*)malloc(400);
    
    getcwd(home_directory, 400);
    getcwd(curr_directory, 400);
    //strcpy(curr_directory, "~\0");
    //printf("curr : %s home :%s\n",curr_directory, home_directory);
    getlogin_r(username, 100);
    gethostname(systemname, 100);
    history curr_history;
    curr_history.index=0;
    curr_history.size=0;
    HistoryReadFromFile(&curr_history);
    //printf("%d\n", getpid());
    while(1){
        StringVector CommandList;
        StringVectorInit(&CommandList);
        
        char *msg1;
        msg1 = pwd();
        printf("<%s@%s:%s>", username, systemname, msg1);
        free(msg1);

        char* input = (char*)malloc(4000);
        fgets(input, 4000, stdin);
        
        if(input[strlen(input)-1]=='\n')
            input[strlen(input)-1]='\0';

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
                    HistoryPrint(commandBreakdown.list[1][0]-'0', &curr_history);
            }
            else if(!strcmp(commandBreakdown.list[0], "exit")){
                HistoryWriteToFile(&curr_history);
                return 0;
            }
            else 
                CommandHandler(&commandBreakdown);
            
            StringVectorErase(&commandBreakdown);
        }        
    }
}

