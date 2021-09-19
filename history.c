#include "utils.h"

void _HistorySwapThing(history *h){
    free(h->his[0]);
    for(int i=0;i<19;i++)
        h->his[i] = h->his[i+1];

    h->size--;
}


void HistoryAdd(char* str, history *h){
    if(h->size!=0){
        if(!strcmp(h->his[h->size-1], str))
            return;
    }
    if(h->size==20){
        _HistorySwapThing(h);
        h->his[19] = (char*)malloc(2*strlen(str)*sizeof(char));
        strcpy(h->his[19],str);
        h->size++;
    }
    else{
        if(h->his[h->size]!=NULL)
            free(h->his[h->size]);
        
        h->his[h->size] = (char*)malloc(2*strlen(str)*sizeof(char));
        strcpy(h->his[h->size],str);
        h->size++;
    }
}

void addToHis(char* str, history *h){
    HistoryAdd(str, h);
}

void HistoryPrint(int cnt, history *h){
    if(cnt==-1){
        for(int i=0;i<h->size;i++)
            printf("%s\n", h->his[i]);
    }
    else {
        int m = cnt;
        if(h->size<m)
            m = h->size;
        
        for(int i=0;i<m;i++)
            printf("%s\n", h->his[i]);
    }
}

void HistoryWriteToFile(history *h){
    FILE *out = fopen("history_storage.txt", "w");

    fprintf(out, "%d\n", h->size);

    for(int i=0;i<h->size;i++){
        fprintf(out, "%s\n", h->his[i]);
    }

    fclose(out);
}

void HistoryReadFromFile(history *h){
    FILE *in = fopen("history_storage.txt", "r");

    if(in==NULL){
        perror("Error opening history_storage.txt");
    }
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
    
    for(int i=0;i<20;i++)
        h->his[i] = NULL;
}