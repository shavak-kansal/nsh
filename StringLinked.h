#ifndef __STRING_LINKED_H_
#define __STRING_LINKED_H_

typedef struct StringLinkedNode {
    char* str;
    int size;
    int pid;
    struct StringLinkedNode *next;
    struct StringLinkedNode *back;
} strLinkNode;

typedef struct StringLinked{
    strLinkNode* head;
    strLinkNode* tail;
} strLink;

void strLinkInit(strLink* list);
void strLinkAdd(strLink* list, char *str, int pidNum);
void strLinkRemove(strLink* list, strLinkNode *inst);
char* StrFindPid(strLink *s, int pid);

#endif