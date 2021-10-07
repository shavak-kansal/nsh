#include "utils.h"

void lsHandler(StringVector *ll){
    //l->size--;
    StringVector *l = (StringVector *)malloc(sizeof(StringVector));
    StringVectorInit(l);
    //StringVectorCopy(ll, l);

    for(int i=0;i<ll->size;i++){
        if(ll->list[i]!=NULL){
            StringVectorAdd(l, ll->list[i]);
        }
    }
    int ind_name=1;

    int flag_a = 0;
    int flag_l = 0;

    if(((flag_a=flag_l=ArgsFinder(l, "-la"))!=-1)||((flag_a=flag_l=ArgsFinder(l, "-la")))!=-1){
        //ind_name++;
    }
    else {
        flag_a = ArgsFinder(l, "-a");
        flag_l = ArgsFinder(l, "-l");
    }


    int num_paths = 0;

    for(int i=0;i<l->size;i++){
        if(((!strcmp("-la", l->list[i]))||(!strcmp("-al", l->list[i]))||(!strcmp("-l", l->list[i]))||(!strcmp("-a", l->list[i]))||(!strcmp("ls", l->list[i]))))
            continue;
        else 
            num_paths++;
    }

    if(num_paths==0){
        StringVectorAdd(l , ".");
        num_paths++;
    }
    for(int ind_name=1;ind_name<l->size;ind_name++){
        int i = ind_name;

        if(((!strcmp("-la", l->list[i]))||(!strcmp("-al", l->list[i]))||(!strcmp("-l", l->list[i]))||(!strcmp("-a", l->list[i]))||(!strcmp("ls", l->list[i]))))
            continue;

        if(l->list[ind_name][0]=='~'){
            char temp[1000];
            temp[0] = 0;
            strcat(temp, home_directory);
            strcat(temp, (l->list[ind_name]+1));
            StringVectorReplace(l, ind_name, temp);
        }           
        int cnt;
        struct dirent** list;
        cnt = scandir(l->list[ind_name], &list, NULL, alphasort);

        if((ind_name==flag_l)||(ind_name==flag_a))
            continue;

        long long total = 0;

        for(int i=0;i<cnt;i++){
            struct stat filePerms;
            char temp[1000];
            sprintf(temp, "%s%s", l->list[ind_name], list[i]->d_name);
            stat(temp, &filePerms);
            total += filePerms.st_blocks;
        }

        //printf("hello");
        printf("%s :\ntotal %lld\n", l->list[ind_name], total/2);

        for(int i=0;i<cnt;i++){
            if(flag_a==-1){
                if(list[i]->d_name[0]=='.')
                    continue;
            }
            if(flag_l!=-1){
                struct stat filePerms;
                char temp[1000];
                sprintf(temp, "%s/%s", l->list[ind_name], list[i]->d_name);
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

                struct tm *timeptr;
                timeptr =  localtime(&(filePerms.st_mtime));
                char* time = (char*)malloc(100*sizeof(char));

                if(timeptr->tm_sec<15780000){    
                    strftime(time, 100,"%b %d %l:%M ",timeptr);
                    printf("%lu\t%s\t%s\t%zu\t%s", filePerms.st_nlink, getpwuid(filePerms.st_uid)->pw_name, getgrgid(filePerms.st_gid)->gr_name, filePerms.st_size, time);
                }
                else {
                    strftime(time, 100,"%b %d %Y ",timeptr);
                    printf("%lu\t%s\t%s\t%zu\t%s", filePerms.st_nlink, getpwuid(filePerms.st_uid)->pw_name, getgrgid(filePerms.st_gid)->gr_name, filePerms.st_size, time);                    
                }
                free(time);
            }
            printf("%s\n", list[i]->d_name);
        }
        printf("\n");

    }    
    StringVectorErase(l);
    free(l);
}