#include "utils.h"

void StringVectorAdd(StringVector* LS, char* str){
    if(LS->size>=LS->maxSize){
        LS->list = (char**)realloc(LS->list, 2*LS->maxSize*sizeof(char*));
        
        LS->maxSize = LS->maxSize*2;

        for(int i=LS->size;i<LS->maxSize;i++)
            LS->list[i] = NULL;
    }

    if(str==NULL){
        LS->list[LS->size] = NULL;
        LS->size++; 
        return;
    }
    LS->list[LS->size] = (char*)malloc(strlen(str)*2);    
    strcpy(LS->list[LS->size], str);
    LS->size++;
}

void StringVectorInit(StringVector* LS){
    LS->maxSize = 2;
    LS->list = (char**)malloc(LS->maxSize*sizeof(char*));
    LS->size = 0;
    
    for(int i=LS->size;i<LS->maxSize;i++)
            LS->list[i] = NULL;
}

void StringVectorErase(StringVector* V){
    for(int i=0;i<V->size;i++){
        free(V->list[i]);
    }

    free(V->list);
    V->size = 0;
    V->maxSize = 0;
}

void StringVectorCopy(StringVector *src, StringVector *dst){
    for(int i=0;i<src->size;i++)
        StringVectorAdd(dst, src->list[i]);
}
