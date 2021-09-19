#include "utils.h"

void p_info(pid_t pid){

    if(pid == -1)
        pid = getpid();

    char* fileAdd = (char*)malloc(200);
    sprintf(fileAdd, "/proc/%d/stat",pid);

    FILE *procStat = fopen(fileAdd, "r");

    if(procStat == NULL){
        printf("Process doesn't exist:");
        return;
    }
    pid_t pgrp, tpgid;
    char pidNo[40], random[100], state[10]; 
    fscanf(procStat, "%s %s %s %s %d %s %s %d", pidNo, random, state, random, &pgrp, random, random, &tpgid);

    if(tpgid==pgrp)
        if((state[0]=='R')||(state[0]=='S'))
            strcat(state, "+");
    
    printf("pid -- %s\nProcess Status -- %s\n", pidNo, state);
    fclose(procStat);
    
    sprintf(fileAdd, "/proc/%d/statm",pid);
    procStat = fopen(fileAdd, "r");
    fscanf(procStat, "%s", random);

    printf("memory -- %s\n Bytes", random);
    fclose(procStat);
    
    readlink("/proc/self/exe", random, 100);
    strcat(random, ".exe");
    printf("Executable Path -- %s\n", random);
}