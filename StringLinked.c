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

void bgJobAdder(strLink* list, char* name, int pid, int jobNum){
    strLinkNode *new = (strLinkNode*)malloc(sizeof(strLinkNode));
    new->pid = pid;
    new->str = (char*)malloc(2*strlen(name)*sizeof(char));
    new->jobNum = jobNum;
    strcpy(new->str, name);

    strLinkNode *temp = list->head;

    while((temp->next!=list->tail)&&(strcmp(temp->next->str, name)<=0))
        temp = temp->next;

    temp->next->back = new;
    new->next=temp->next;
    new->back=temp;
    temp->next = new;
}

char* bgJobRemove(strLink* list, int pid){
    strLinkNode *temp = list->head;
    
    while(temp->next!=list->tail){
        if(temp->next->pid==pid){
            strLinkNode *temp1 = temp->next;

            

            strLinkNode *temp2 = list->head;
            while(temp2->next!=list->tail){
                
                if(temp2->next->jobNum>temp->next->jobNum)
                    temp2->next->jobNum--;

                temp2 = temp2->next;
            }
            //temp->next = temp->next->next;
            //temp->next->back = temp;

            temp->next = temp1->next;
            temp1->back = temp;

            char *str = malloc(2*strlen(temp1->str)*sizeof(char));
            strcpy(str, temp1->str);
            free(temp1->str);
            free(temp1);
            return str;
        }
        temp = temp->next;
    }
    
    return NULL;
}

void strLinkAdd(strLink* list, char *str, int pidNum){

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

int StrFindNum(strLink *s, int job){
    strLinkNode* start = s->head;
    
    while(start!=s->tail){
        if(start->jobNum==job){
            return start->pid;
            break;
        }
        start=start->next;
    }

    return -1;
}
