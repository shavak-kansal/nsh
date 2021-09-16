#include "utils.h"
#include "StringLinked.c"

int main(){
    strLink test;

    strLinkInit(&test);
    strLinkAdd(&test, "hello1", 41);
    strLinkAdd(&test, "hello2", 29);
    strLinkAdd(&test, "hello3", 100);

    strLinkNode* start = test.head;

    while(start->next!=test.tail){
        printf("pid : %d : string : %s\n", start->next->pid, start->next->str);
        start = start->next;
    }

    start = test.head;

    while(start!=test.tail){
        if(start->pid==100){
            strLinkRemove(&test, start);
            break;
        }
        start=start->next;
    }
    
    start = test.head;

    while(start->next!=test.tail){
        printf("pid : %d : string : %s\n", start->next->pid, start->next->str);
        start = start->next;
    }
}