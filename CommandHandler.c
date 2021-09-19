#include "utils.h"
#include "functions.h"

extern char *username;
extern char *systemname;

strLink bgProcessList;
char last_dir[300];
void handler(){
    int status;
    int pid;

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
            msg1 = pwd();
            printf("<%s@%s:%s>", username, systemname, msg1);
            free(msg1);
            fflush(stdout);
        }
    }
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

        int ret; 
        if(l->size==1)
            return;
        else if(!strcmp(l->list[1], "~"))
            ret = chdir(home_directory);
        else if(!strcmp(l->list[1], "-"))
            ret = chdir(last_dir);
        else 
            ret = chdir(l->list[1]);
        
        if(ret==-1)
            perror("Error with cd ");
        else 
            strcpy(last_dir,curr_directory);        
        getcwd(curr_directory, 100);

    }

    else if(!strcmp(l->list[0],"pwd")){
    
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

        for(int i=0;i<(atoi(l->list[1]));i++){
            CommandHandler(&l_r);  
        }

        StringVectorErase(&l_r);
    }
    else if(!strcmp(l->list[0], "pinfo")){
        if(l->size>1)
            p_info(atoi(l->list[1]));
        else 
            p_info(-1);
    }
    else{
        
        int dontwait = 0;
        int index;
        
        if((index=ArgsFinder(l, "&"))!=-1)
            dontwait = 1;

        int pid = fork();
        
        if(pid==-1){
            perror("Fork error");
        }
        else if(pid==0){
            setpgid(0,0);
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