#include "utils.h"

int HistoryPrint(int cnt, history *h){
    
    if(cnt==-1){
        if(h->index<20){
            for(int i=0;i<h->index;i++)
                printf("%s\n", h->his[i]);
        }
        else{
            for(int i=0;i<20;i++)
                printf("%s", h->his[(h->index+i+19)%20]);
        }
    }
    else{
        for(int i=0;i<cnt;i++)
            printf("%s\n", h->his[(h->index-i+19)%20]);
    }
}

void addToHis(char* str, history *h){
    if(h->his[(h->index+19)%20]!=NULL){
        if(!strcmp(str, h->his[(h->index+19)%20]))
            return;
    }

    if(h->his[(h->index)%20]!=NULL)
        free(h->his[(h->index)%20]);
    h->his[(h->index)%20] = (char*)malloc(2*strlen(str)*sizeof(char));
    strcpy(h->his[(h->index)%20], str);
    h->index++;
    if(h->size<20)
    h->size++;
}

void HistoryWriteToFile(history *h){
    FILE *out = fopen("history_storage.txt", "w");

    fprintf(out, "%d\n", h->index);
    fprintf(out, "%d\n", h->size);

    for(int i=0;i<h->size;i++){
        fprintf(out, "%s\n", h->his[i]);
    }

    fclose(out);
}

void HistoryReadFromFile(history *h){
    FILE *in = fopen("history_storage.txt", "r");

    fscanf(in, "%d\n", &h->index);
    fscanf(in, "%d\n", &h->size);

    for(int i=0;i<h->size;i++){
        //char *temp;
        char **lineptr = (char**)malloc(sizeof(char*));
        size_t line_size = 999;
        *lineptr = NULL;
        //fscanf(in, "%s\n", temp);
        getline(lineptr, &line_size, in);
        h->his[i] = (char*)malloc(2*strlen(*lineptr)*sizeof(char));
        strcpy(h->his[i], *lineptr);
        h->his[i][strcspn(h->his[i], "\n")] = 0;
        free(*lineptr);
        free(lineptr);
    }

    fclose(in);
}

void HistoryInit(history *h){
    h->size=0;
    h->index=0;
    
    for(int i=0;i<20;i++)
        h->his[i] = NULL;
}