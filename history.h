#ifndef __HISTORY_H_
#define __HISTORY_H_

#include "utils.h"

typedef struct _command_history{
    char* his[20];
    int size;
} history;

void addToHis(char* str, history *h);
void HistoryPrint(int cnt, history *h);
void HistoryWriteToFile(history *h);
void HistoryReadFromFile(history *h);
void HistoryInit(history *h);

#endif