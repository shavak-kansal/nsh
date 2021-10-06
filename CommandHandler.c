#include "utils.h"
#include "functions.h"

extern char *username;
extern char *systemname;
extern int maxJobNum;

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

char pid_state(int pid){
    char* fileAdd = (char*)malloc(200);
    sprintf(fileAdd, "/proc/%d/stat",pid);

    FILE *procStat = fopen(fileAdd, "r");
    char state[10]; 
    fscanf(procStat, "%s %s %s", state, state, state);

    return state[0];
}
void CommandHandler(StringVector *l){

    if(!strcmp(l->list[0],"jobs")){

        int rFlag = ArgsFinder(l, "-r");
        int sFlag = ArgsFinder(l, "-s");
        
        strLinkNode* step = bgProcessList.head;

        if((rFlag==-1)&&(sFlag==-1))
            while(step->next!=bgProcessList.tail){
                step = step->next;
                char state = pid_state(step->pid);
                char msg[30];

                if(state=='R')
                    strcpy(msg, "Running");
                else 
                    strcpy(msg, "Stopped");
                printf("[%d] %s %s [%d]\n", step->jobNum,msg ,step->str, step->pid);
            }
        else if(rFlag!=-1){
            while(step->next!=bgProcessList.tail){
                step = step->next;
                if(pid_state(step->pid)=='R')
                    printf("Running [%d] %s [%d]\n", step->jobNum, step->str, step->pid);
            }
        }
        else if(sFlag!=-1){
            while(step->next!=bgProcessList.tail){
                step = step->next;
                char state = pid_state(step->pid);
                if(pid_state(step->pid)=='S')
                    printf("Stopped [%d] %s [%d]\n", step->jobNum, step->str, step->pid);
            }
        }
    }
    else if(!strcmp(l->list[0],"echo")){

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
    else if(!strcmp(l->list[0], "test")){

        StringVector list[4];

        for(int i=0;i<4;i++)
            StringVectorInit(&list[i]);

        StringVectorAdd(&list[0], "ls");
        StringVectorAdd(&list[0], "-l");
        StringVectorAdd(&list[0], 0);

        StringVectorAdd(&list[1], "awk");
        StringVectorAdd(&list[1], "{print $1}");
        StringVectorAdd(&list[1], 0);

        StringVectorAdd(&list[2], "sort");
        StringVectorAdd(&list[2], 0);

        StringVectorAdd(&list[3], "uniq");
        StringVectorAdd(&list[3],0);

        pipingHandler(list, 4);
    }
    else{
        
        int bgflag = 0;
        int index;
        
        if((index=ArgsFinder(l, "&"))!=-1)
            bgflag = 1;

        int pid = fork();
        
        if(pid==-1){
            perror("Fork error");
        }
        else if(pid==0){
            if(bgflag)
                setpgid(0,0);
            
            if(bgflag){
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
            
            if(bgflag){
                bgJobAdder(&bgProcessList, l->list[0], pid, maxJobNum);//strLinkAdd(&bgProcessList, l->list[0], pid);
                maxJobNum++;
            }
            else 
                waitpid(pid, NULL, 0);
            //signal(SIGCHLD, handler);            
        }
    }
    
}