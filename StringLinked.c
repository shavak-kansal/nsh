#include "utils.h"
#include "StringLinked.h"

void strLinkInit(strLink* list){
    list->head = (strLinkNode*)malloc(sizeof(strLinkNode));
    list->tail = (strLinkNode*)malloc(sizeof(strLinkNode));
    //list->head=NULL;
    list->head->next=list->tail;
    list->head->back=NULL;
    //list->tail=NULL;
    list->tail->next=NULL;
    list->tail->back=list->head;
}

void strLinkAdd(strLink* list, char *str, int pidNum){
    strLinkNode *temp = list->tail->back;

    strLinkNode *new = (strLinkNode*)malloc(sizeof(strLinkNode));
    new->pid = pidNum;
    new->str = (char*)malloc(strlen(str)*sizeof(char));
    strcpy(new->str, str);

    new->next=list->tail;
    new->back=temp;

    list->tail->back=new;
    
    temp->next=new;
}

void strLinkRemove(strLink* list, strLinkNode *inst){
    strLinkNode *back = inst->back;
    strLinkNode *forw = inst->next;

    forw->back = back;
    back->next = forw;

    free(inst);
}

char* StrFindPid(strLink *s, int pid){
    strLinkNode* start = s->head;
    
    char* str1;

    while(start!=s->tail){
        if(start->pid==pid){
            str1 = start->str;
            strLinkRemove(s, start);
            break;
        }
        start=start->next;
    }

    return str1;
}
