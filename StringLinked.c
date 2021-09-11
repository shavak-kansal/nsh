#include <stdlib.h>

typedef struct StringLinked{
    strLinkNode* head;
    strLinkNode* tail;
} strLink;

typedef struct StringLinkedNode {
    char* str;
    int size;
    struct StringLinkedNode *next;
    struct StringLinkedNode *back;
} strLinkNode;

void strLinkInit(strLink* list){
    list->head=NULL;
    list->head->next=list->tail;
    list->head->back=NULL;
    list->tail=NULL;
    list->tail->next=NULL;
    list->tail->back=list->head;
}

void strLinkAdd(strLink* list){

}

void strLinkPop(strLink* list){

}
