#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include "StringVector.c"

char *curr_directory;
char *home_directory;

int args_finder(char* command_breakdown[50], char *arg, int cnt){

    for(int i=0;i<cnt;i++){
        if(!strcmp(command_breakdown[i], arg))
            return i;
    }

    return -1;
}

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

void ls_handler(char* command_breakdown[50], int count_args){
    int ind_name=1;

    int flag_a = 0;
    int flag_l = 0;

    if(((flag_a=flag_l=args_finder(command_breakdown, "-la", count_args))!=-1)||((flag_a=flag_l=args_finder(command_breakdown, "-al", count_args)))!=-1){
        ind_name++;
    }
    else {
        flag_a = args_finder(command_breakdown, "-a", count_args);
        flag_l = args_finder(command_breakdown, "-l", count_args);
    }

    StringVector Directories;
    StringVectorInit(&Directories);

    for(int ind_name=1;ind_name<count_args;ind_name++){
        int cnt;
        struct dirent** list;
        cnt = scandir(command_breakdown[ind_name], &list, NULL, alphasort);

        if((ind_name==flag_l)||(ind_name==flag_a))
            continue;

        printf("%s :\n", command_breakdown[ind_name]);
        
        for(int i=0;i<cnt;i++){
            if(flag_a==-1){
                if(list[i]->d_name[0]=='.')
                    continue;
            }
            if(flag_l!=-1){
                struct stat filePerms;
                char temp[1000];
                sprintf(temp, "%s%s", command_breakdown[ind_name], list[i]->d_name);
                stat(temp, &filePerms);
                printf( (S_ISDIR(filePerms.st_mode)) ? "d" : "-");
                printf( (filePerms.st_mode & S_IRUSR) ? "r" : "-");
                printf( (filePerms.st_mode & S_IWUSR) ? "w" : "-");
                printf( (filePerms.st_mode & S_IXUSR) ? "x" : "-");
                printf( (filePerms.st_mode & S_IRGRP) ? "r" : "-");
                printf( (filePerms.st_mode & S_IWGRP) ? "w" : "-");
                printf( (filePerms.st_mode & S_IXGRP) ? "x" : "-");
                printf( (filePerms.st_mode & S_IROTH) ? "r" : "-");
                printf( (filePerms.st_mode & S_IWOTH) ? "w" : "-");
                printf( (filePerms.st_mode & S_IXOTH) ? "x" : "-");
                printf(" ");
                printf("%d %s %s ", filePerms.st_nlink, getpwuid(filePerms.st_uid)->pw_name, getgrgid(filePerms.st_gid)->gr_name/* , ctime(filePerms.c) */);

            }
            printf("%s\n", list[i]->d_name);
        }
    }    
}

void command_handler(char* command_breakdown[50], int count_args){

    if(!strcmp(command_breakdown[0],"echo")){
        int i=1;
        while(command_breakdown[i]!=NULL){
            printf("%s", command_breakdown[i]);
            i++;
        }
        printf("\n");
    }
    else if(!strcmp(command_breakdown[0],"cd")){
        chdir(command_breakdown[1]);
        getcwd(curr_directory, 100);

        if(!strcmp(home_directory, curr_directory)){
            curr_directory[0]='~';
            curr_directory[1]='\0';
        }
    }
    else if(!strcmp(command_breakdown[0],"pwd")){
        if(curr_directory[0]!='~')
            printf("%s\n", curr_directory);
        else 
            printf("%s\n", home_directory);
    }
    else if(!strcmp(command_breakdown[0], "ls")){
        ls_handler(command_breakdown,count_args);
    }
    else if(!strcmp(command_breakdown[0], "repeat")){
        for(int i=0;i<(command_breakdown[1][0]-'0');i++){
            command_handler(command_breakdown + 2, count_args - 2);
        }
    }
    else{
        
        int flag = 1;

        if(args_finder(command_breakdown, "&", count_args)!=-1)
            flag = 0;

        int pid = fork();
        if(pid==0)
            execvp(command_breakdown[0], (command_breakdown+1));
        
        if(flag)
            while(wait(NULL)>0);
    }
    
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
    
    while(1){
        StringVector CommandList;
        StringVectorInit(&CommandList);
        
        printf("<%s@%s:%s>", username, systemname, curr_directory);
        
        char* input = (char*)malloc(4000);
        fgets(input, 4000, stdin);
        
        if(input[strlen(input)-1]=='\n')
            input[strlen(input)-1]='\0';

        int count = InputSanitize(input, &CommandList, ";");
        free(input);

        for(int i=0;i<count;i++){
            StringVector commandBreakdown;
            StringVectorInit(&commandBreakdown);
            int count_args = InputSanitize(CommandList.list[i], &commandBreakdown, " \t");

            for(int i=0;i<commandBreakdown.size;i++){
                printf("%s\n", commandBreakdown.list[i]);
            }
            printf("\n");
            //command_handler(command_breakdown, count_args);
        }        
    }
}

