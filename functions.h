#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include "utils.h"

int ArgsFinder(StringVector* l, char *arg);
int betterArgsFinder(StringVector* l, char *arg, int start);
void CommandHandler(StringVector *l);
void lsHandler(StringVector *l);
void p_info(pid_t pid);
char* pwd();
void prompt();
void pipingHandler(StringVector* list, int size);
int ImprovedCommandHandler(StringVector *l);

char pid_state(int pid);
#endif