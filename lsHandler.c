#include "utils.h"

void lsHandler(StringVector *l){
    int ind_name=1;

    int flag_a = 0;
    int flag_l = 0;

    if(((flag_a=flag_l=ArgsFinder(l, "-la"))!=-1)||((flag_a=flag_l=ArgsFinder(l, "-la")))!=-1){
        ind_name++;
    }
    else {
        flag_a = ArgsFinder(l, "-a");
        flag_l = ArgsFinder(l, "-l");
    }

    for(int ind_name=1;ind_name<l->size;ind_name++){
        int cnt;
        struct dirent** list;
        cnt = scandir(l->list[ind_name], &list, NULL, alphasort);

        if((ind_name==flag_l)||(ind_name==flag_a))
            continue;

        printf("%s :\n", l->list[ind_name]);
        
        for(int i=0;i<cnt;i++){
            if(flag_a==-1){
                if(list[i]->d_name[0]=='.')
                    continue;
            }
            if(flag_l!=-1){
                struct stat filePerms;
                char temp[1000];
                sprintf(temp, "%s%s", l->list[ind_name], list[i]->d_name);
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
                strftime(time, 100,"%b %d %l:%M ",timeptr);
                printf("%lu\t%s\t%s\t%ld\t%s", filePerms.st_nlink, getpwuid(filePerms.st_uid)->pw_name, getgrgid(filePerms.st_gid)->gr_name, filePerms.st_size, time);

            }
            printf("%s\n", list[i]->d_name);
        }
        printf("\n");
    }    
}