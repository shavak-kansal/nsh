#include "utils.h"
#include "functions.h"

strLink bgProcessList;

void handler(){
    int status;
    int pid = wait(&status);

    char *name = StrFindPid(&bgProcessList, pid);
    char msg[20];

    if(status==0)
        strcpy(msg, "normally");
    else 
        strcpy(msg, "abnormally");
        
    printf("%s with %d exited %s\n", name, pid, msg);
}
void CommandHandler(StringVector *l){

    if(!strcmp(l->list[0],"echo")){

        int i=1;
        
        while(i<l->size){
            printf("%s", l->list[i]);
            i++;
        }

        printf("\n");
    }
    
    else if(!strcmp(l->list[0],"cd")){
        chdir(l->list[1]);

        getcwd(curr_directory, 100);

        if(!strcmp(home_directory, curr_directory))
            strcpy(curr_directory, "~");

    }

    else if(!strcmp(l->list[0],"pwd")){
    
        // if(curr_directory[0]!='~')
        //     printf("%s\n", curr_directory);
        // else 
        //     printf("%s\n", home_directory);
        char *msg = pwd();
        printf("%s\n", msg);
        free(msg);
    }

    else if(!strcmp(l->list[0], "ls")){
        lsHandler(l);
    }

    else if(!strcmp(l->list[0], "repeat")){    
        StringVector l_r;
        StringVectorInit(&l_r);
        
        for(int i=2;i<l->size;i++)
            StringVectorAdd(&l_r, l->list[i]);

        for(int i=0;i<(l->list[1][0]-'0');i++){
            CommandHandler(&l_r);  
        }

        StringVectorErase(&l_r);
    }
    else if(!strcmp(l->list[0], "pinfo")){
        if(l->size>1)
            p_info(l->list[1][0]-'0');
        else 
            p_info(-1);
    }
    else{
        
        int dontwait = 0;
        int index;
        
        if((index=ArgsFinder(l, "&"))!=-1)
            dontwait = 1;

        int pid = fork();

        if(pid==0){
            if(dontwait){
                free(l->list[index]);
                l->list[index] = NULL;
            }
            else 
                StringVectorAdd(l, NULL);

            if(execvp(l->list[0], l->list)<0){
                perror("Execvp error : ");
            }
            exit(0);
        }
        else{
            
            if(dontwait){
                strLinkAdd(&bgProcessList, l->list[0], pid);
                signal(SIGCHLD, handler);
            }

            if(!dontwait)
                wait(NULL);
        }
    }
    
}