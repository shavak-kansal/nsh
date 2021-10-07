#include "utils.h"
#include "functions.h"

extern char *username;
extern char *systemname;
extern int maxJobNum;

extern strLink bgProcessList;

int compareStr(char *str1 , char *str2){
    for(int i=0;i<strlen(str2);i++){
        if(str1[i]!=str2[i]){
            return 0;
        }
    }
    return 1;
}
int ImprovedCommandHandler(StringVector *ll){
    StringVector *l = (StringVector*)malloc(sizeof(StringVector));
    StringVectorInit(l);
    // StringVectorCopy(ll, l);
    
    StringVectorAdd(l, "ls");
    StringVectorAdd(l, "-l");
    StringVectorAdd(l, 0);
    if(compareStr(l->list[0],"jobs")){

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

        _exit(0);
    }
    
    else if(compareStr(l->list[0],"echo")){

        int i=1;
        
        while(i<l->size){
            printf("%s", l->list[i]);
            i++;
        }

        printf("\n");

        _exit(0);
    }
    
    else if(compareStr(l->list[0],"pwd")){
    
        char *msg = pwd();
        printf("%s\n", msg);
        free(msg);
        _exit(0);
    }

    else if(compareStr(l->list[0], "ls")){
        lsHandler(l);
    }
    
    else if(compareStr(l->list[0], "pinfo")){
        if(l->size>1)
            p_info(atoi(l->list[1]));
        else 
            p_info(-1);

        _exit(0);
    }

    else if(compareStr(l->list[0], "repeat")){    
        int count = atoi(l->list[1]);

        for(int i=0;i<count;i++){

            if(compareStr(l->list[2],"jobs")){
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
            else if(compareStr(l->list[2],"echo")){

                int i=3;
                
                while(i<l->size){
                    printf("%s", l->list[i]);
                    i++;
                }

                printf("\n");
            }
        
            else if(compareStr(l->list[0],"pwd")){
            
                char *msg = pwd();
                printf("%s\n", msg);
                free(msg);
            }

            else if(compareStr(l->list[2], "ls")){
                StringVector cpy;
                StringVectorInit(&cpy);

                for(int i=2;i<l->size;i++)
                    StringVectorAdd(&cpy, l->list[i]);

                lsHandler(&cpy);

                StringVectorErase(&cpy);
            }
        }
        _exit(0);    
    }
    
    else return 1;
    
}