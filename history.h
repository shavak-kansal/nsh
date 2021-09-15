#ifndef __HISTORY_H_
#define __HISTORY_H_

#include "utils.h"

typedef struct _command_history{
    char* his[20];
    int index;
} history;

void addToHis(char* str, history *h);
int print(int cnt, history *h);

#endif