#include "utils.h"

int ArgsFinder(StringVector* l, char *arg){

    for(int i=0;i<l->size;i++){
        if(!strcmp(l->list[i], arg))
            return i;
    }

    return -1;
}