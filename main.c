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

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}


void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


char* InputHandler() {
    //char* his[4] = {"monkay", "brute", "juice", "filler"};

    char *inp = malloc(sizeof(char) * 100);
    char c;
    while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int index = curr_history.size;
        int pt = 0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) break;
                else if (c == 27) {
                    index--;
                    printf("%s", curr_history.his[index]);
                    char d;
                    while (read(STDIN_FILENO, &d, 1) == 1) { // length of escape code
                        if(d == 32){
                            printf(" ");
                            for(int i=0;i<strlen(curr_history.his[index]);i++)
                                inp[pt++] = curr_history.his[index][i];

                            inp[pt++] = ' ';
                            
                            break;    
                        }
                        else if(d==27){
                            for(int i=0;i<strlen(curr_history.his[index]);i++)
                                printf("\b \b");

                            if(index>0)
                                index--;
                            
                            printf("%s", curr_history.his[index]);
                        }
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        return inp;
    }
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
        int pid;
        int status;
        while((pid = waitpid(-1,&status, WNOHANG | WUNTRACED))>0){

            char *name = StrFindPid(&bgProcessList, pid);

            if(name!=NULL){
                char msg[20];

                if(status==0)
                    strcpy(msg, "normally");
                else 
                    strcpy(msg, "abnormally");
                    
                printf("%s with %d exited %s\n", name, pid, msg);
                char *msg1;
                fflush(stdout);
            }
        }

        StringVector CommandList;
        StringVectorInit(&CommandList);
        
        char *msg1;
        msg1 = pwd();
        printf("<%s@%s:%s>", username, systemname, msg1);
        free(msg1);

        char* input = NULL/* (char*)malloc(4000) */;
        //free(input);
        //fgets(input, 4000, stdin);
        input = InputHandler();
        if(!strcmp(input,"")){
            printf("\n");
            return;
        }
        printf("\n");
        
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
                exit(0);
            }
            else{
                
                CommandHandler(&commandBreakdown);
            }
            StringVectorErase(&commandBreakdown);
        }
}

