#include "utils.h"

extern strLink bgProcessList;

int boring(StringVector* l) {
    if(!strcmp(l->list[0],"jobs")){
        return 0;
    }
    else if(!strcmp(l->list[0],"echo")){
        return 0;
    }
    
    else if(!strcmp(l->list[0],"pwd")){
        return 0;
    }

    else if(!strcmp(l->list[0], "ls")){
        return 0;
    }
    
    else if(!strcmp(l->list[0], "pinfo")){
        return 0;
    }

    else return 1;
}

void pipingHandler(StringVector* list, int size){

    int pid;
    int in, out, fd1[2], fd2[2];

    int in1, out1;
    pipe(fd2);

    in = dup(0);
    out = dup(1);

    if(boring(&(list[0]))==1){
        if((pid = fork())==0){
            
            close(fd2[0]);
            dup2(fd2[1],1);
            close(fd2[1]);

            StringVectorAdd(&list[0], NULL);
            execvp(list[0].list[0], list[0].list);
        }
        else if(pid>0){
            waitpid(pid, NULL, 0);
            fd1[0] = fd2[0];
            fd1[1] = fd2[1];
        }
    }
    else {
        if((pid = fork())==0){
            
            close(fd2[0]);
            dup2(fd2[1],1);
            close(fd2[1]);
            
            ImprovedCommandHandler(&(list[0]));
            _exit(0);
        }
        else if(pid>0){
            waitpid(pid, NULL, 0);
            fd1[0] = fd2[0];
            fd1[1] = fd2[1];
        }
    }

    for(int i=1;i<size-1;i++){
        pipe(fd2);

        if(boring(&(list[i]))==1){
            if((pid = fork())==0){
                dup2(fd1[0],0);
                close(fd1[0]);
                close(fd1[1]);

                close(fd2[0]);
                dup2(fd2[1],1);
                close(fd2[1]);

                StringVectorAdd(&list[i], NULL);
                execvp(list[i].list[0], list[i].list);
                _exit(0);
            }
            else if(pid>0){
                //waitpid(pid, NULL, 0);
                close(fd1[0]);
                close(fd1[1]);

                fd1[0] = fd2[0];
                fd1[1] = fd2[1];
            }
        }
        else{
            if((pid = fork())==0){
                dup2(fd1[0],0);
                close(fd1[0]);
                close(fd1[1]);

                close(fd2[0]);
                dup2(fd2[1],1);
                close(fd2[1]);


                ImprovedCommandHandler(&(list[i]));
                _exit(0);
            }
            else if(pid>0){
                //waitpid(pid, NULL, 0);
                close(fd1[0]);
                close(fd1[1]);

                fd1[0] = fd2[0];
                fd1[1] = fd2[1];
            }
        }
    }
    if(boring(&(list[size-1]))==1){
        if((pid = fork())==0){
            dup2(fd1[0],0);
            close(fd1[0]);
            close(fd1[1]);

            StringVectorAdd(&list[size-1], NULL);
            execvp(list[size-1].list[0], list[size-1].list);
        }
        else if(pid>0){
            //printf("Child process %d\n", pid);
            //waitpid(pid, NULL, 0);
            close(fd1[0]);
            close(fd1[1]);
        }
    }
    else {
        if((pid = fork())==0){
            dup2(fd1[0],0);
            close(fd1[0]);
            close(fd1[1]);

            ImprovedCommandHandler(&(list[size-1]));
            _exit(0);
        }
        else if(pid>0){
            //printf("Child process %d\n", pid);
            //waitpid(pid, NULL, 0);
            close(fd1[0]);
            close(fd1[1]);
        }
    }
    // close(fd1[0]);
    // close(fd1[1]);
}