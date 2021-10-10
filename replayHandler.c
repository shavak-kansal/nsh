#include "utils.h"

void ReplayFunction(StringVector* command, int interval, int period){
    
    int cnt = period/interval;

    for(int i=0;i<cnt;i++){
        sleep(interval);
        CommandHandler(command);
    }
}