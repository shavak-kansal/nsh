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
    //strLinkNode *temp = list->tail->back;

    strLinkNode *new = (strLinkNode*)malloc(sizeof(strLinkNode));
    new->pid = pidNum;
    new->str = (char*)malloc(2*strlen(str)*sizeof(char));
    strcpy(new->str, str);

    strLinkNode *temp = list->head;

    while((temp->next!=list->tail)&&(strcmp(temp->next->str, str)<=0))
        temp = temp->next;

    temp->next->back = new;
    new->next=temp->next;
    new->back=temp;
    temp->next = new;

}

void strLinkRemove(strLink* list, strLinkNode *inst){
    strLinkNode *back = inst->back;
    strLinkNode *forw = inst->next;

    forw->back = back;
    back->next = forw;

    free(inst->str);
    free(inst);
}

char* StrFindPid(strLink *s, int pid){
    strLinkNode* start = s->head;
    
    char* str1 = NULL;

    while(start!=s->tail){
        if(start->pid==pid){
            str1 = (char*)malloc(2*strlen(start->str)*sizeof(char));
            strcpy(str1, start->str);
            //str1 = start->str;
            strLinkRemove(s, start);
            break;
        }
        start=start->next;
    }

    return str1;
}
