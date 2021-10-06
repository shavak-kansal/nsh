#include "utils.h"

void pipingHandler(StringVector* list, int size){

    int pid;
    int in, out, fd1[2], fd2[2];

    pipe(fd2);

    if((pid = fork())==0){
        close(fd2[0]);
        dup2(fd2[1],1);
        close(fd2[1]);
        if(ImprovedCommandHandler(&(list[0]))==-1)
            execvp(list[0].list[0], list[0].list);
    }
    else if(pid>0){
        fd1[0] = fd2[0];
        fd1[1] = fd2[1];
    }

    for(int i=1;i<size-1;i++){
        pipe(fd2);

        if((pid = fork())==0){
            dup2(fd1[0],0);
            close(fd1[0]);
            close(fd1[1]);

            close(fd2[0]);
            dup2(fd2[1],1);
            close(fd2[1]);

            if(ImprovedCommandHandler(&(list[i]))==-1)
                execvp(list[i].list[0], list[i].list);
        }
        else if(pid>0){
            close(fd1[0]);
            close(fd1[1]);

            fd1[0] = fd2[0];
            fd1[1] = fd2[1];
        }
    }

    if((pid = fork())==0){
        dup2(fd1[0],0);
        close(fd1[0]);
        close(fd1[1]);
        if(ImprovedCommandHandler(&(list[0]))==-1)
            execvp(list[size-1].list[0], list[size-1].list);
    }
    else if(pid>0){
        close(fd1[0]);
        close(fd1[1]);
    }

    close(fd1[0]);
    close(fd1[1]);
}