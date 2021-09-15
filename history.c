#include "utils.h"

int HistoryPrint(int cnt, history *h){
    
    if(cnt==-1){
        if(h->index<20){
            for(int i=0;i<h->index;i++)
                printf("%s\n", h->his[i]);
        }
        else{
            for(int i=0;i<20;i++)
                printf("%s", h->his[(h->index+i-1)%20]);
        }
    }
    else{
        for(int i=0;i<cnt;i++)
            printf("%s\n", h->his[(h->index-i-1)%20]);
    }
}

void addToHis(char* str, history *h){
    
    h->his[(h->index)%20] = (char*)malloc(2*strlen(str)*sizeof(char));
    strcpy(h->his[(h->index)%20], str);
    h->index++;
}