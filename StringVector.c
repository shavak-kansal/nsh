#include "StringVector.h"

void StringVectorAdd(StringVector* LS, char* str){
    if(LS->size>=LS->maxSize){
        LS->list = (char**)realloc(LS->list, 2*LS->maxSize*sizeof(char*));
        LS->maxSize = LS->maxSize*2;
    }
    LS->list[LS->size] = (char*)malloc(strlen(str)*2);    
    strcpy(LS->list[LS->size], str);
    LS->size++;
}

void StringVectorInit(StringVector* LS){
    LS->maxSize = 2;
    LS->list = (char**)malloc(LS->maxSize*sizeof(char*));
    LS->size = 0;
}

void StringVectorErase(StringVector* V){
    for(int i=0;i<V->size;i++){
        free(V->list[i]);
    }

    free(V->list);
}

