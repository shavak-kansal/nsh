#include "utils.h"

int ArgsFinder(StringVector* l, char *arg){

    for(int i=0;i<l->size;i++){
        if(l->list[i]!=NULL)
            if(!strcmp(l->list[i], arg))
                return i;
    }

    return -1;
}

int betterArgsFinder(StringVector* l, char *arg, int start){

    int cnt = 0;

    for(int i=start;i<l->size;i++){
        if(!strcmp(l->list[i], arg))
            cnt++;
    }

    return cnt;
}