#include "utils.h"
#include "functions.h"

extern char *username;
extern char *systemname;
extern int maxJobNum;
extern int foregroundPid;

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

                if(state!='T')
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

        char *pain;
        long num = strtol(l->list[1],&pain,10);
        if (pain[0] != '\0')
        {
            printf("Enter a numerical value\n");
            return;
        }

        for(int i=0;i<num;i++){
            CommandHandler(&l_r);  
        }

        StringVectorErase(&l_r);
    }
    else if(!strcmp(l->list[0], "pinfo")){
        if(l->size>1){
            char *pain;
            long num = strtol(l->list[1],&pain,10);
            if (pain[0] != '\0')
            {
                printf("Enter a numerical value\n");
                return;
            }
            p_info(num);
        }
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
    else if(!strcmp(l->list[0], "sig")){
        int jobNum = strtol(l->list[1], NULL, 10);
        int sig = strtol(l->list[2], NULL, 10);

        int pid = StrFindNum(&bgProcessList, jobNum);
        
        if(kill(pid, sig)<0)
            perror("Error with signal specified");
    }
    else if(!strcmp(l->list[0], "fg")){
        int jobNum = strtol(l->list[1], NULL, 10);
        int pid = StrFindNum(&bgProcessList, jobNum);
        int pid1 = getpid();
        bgJobRemove(&bgProcessList, pid);
        maxJobNum--;

        //tcsetpgrp(0, pid);
        if(kill(pid, SIGCONT)<0)
            perror("Error with signal specified");
            
        foregroundPid = pid;
        waitpid(pid, NULL, WUNTRACED);
        //tcsetpgrp(0, pid1);

    }

    else if(!strcmp(l->list[0], "bg")){
        int jobNum = strtol(l->list[1], NULL, 10);
        if(jobNum==-1){
            printf("No such job\n");
            return;
        }
        int pid = StrFindNum(&bgProcessList, jobNum);

        if(kill(pid, SIGCONT)<0)
            perror("Error with signal specified");
    }
    else if(!strcmp(l->list[0], "replay")){
        int interval;
        int period;
        
        StringVectorDelete(l, 0);
        int index1 = ArgsFinder(l, "-interval");
        StringVectorDelete(l, index1);
        interval = strtol(l->list[index1], NULL, 10); 
        StringVectorDelete(l, index1);

        index1 = ArgsFinder(l, "-period");
        StringVectorDelete(l, index1);
        period = strtol(l->list[index1], NULL, 10);
        StringVectorDelete(l, index1);

        index1 = ArgsFinder(l, "-command");
        StringVectorDelete(l, index1);

        ReplayFunction(l, interval, period);
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
                
            if(bgflag){
                setpgid(0,0);
                free(l->list[index]);
                l->list[index] = NULL;
            }
            else {
                StringVectorAdd(l, NULL);
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
            }
            if(execvp(l->list[0], l->list)<0){
                perror("Execvp error ");
            }
            _exit(0);
        }
        else{
            //bgJobAdder(&bgProcessList, l->list[0], pid, maxJobNum);

            if(bgflag){
                bgJobAdder(&bgProcessList, l->list[0], pid, maxJobNum);//strLinkAdd(&bgProcessList, l->list[0], pid);
                maxJobNum++;
            }
            else{ 
            	tcsetpgrp(0, pid);
                signal(SIGTTOU, SIG_IGN);
                waitpid(pid, NULL, WUNTRACED);
                bgJobRemove(&bgProcessList,pid);
                signal(SIGTTOU, SIG_DFL);
                tcsetpgrp(0, getpgrp());
            }
            //signal(SIGCHLD, handler);            
        }
    }
    
}