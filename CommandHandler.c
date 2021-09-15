#include "utils.h"
#include "functions.h"

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
    
        if(curr_directory[0]!='~')
            printf("%s\n", curr_directory);
        else 
            printf("%s\n", home_directory);
    
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

        if(ArgsFinder(l, "&")!=-1)
            dontwait = 1;

        int pid = fork();
        
        if(pid==0){
            //printf("I am child\n");

            StringVectorAdd(l, NULL);

            if(execvp(l->list[0], l->list)<0){
                perror("Execvp error : ");
                printf("This shouldn't print\n");
                return;
            }
            // printf("Error execvp\n");
            //exit(0);
        }
        else{
            printf("child process, pid : %d\n", pid);
            
            wait(NULL);
        }
    }
    
}