#include "utils.h"

char* pwd(){
        char* prompt;
        prompt = (char*)malloc(2*strlen(curr_directory)*sizeof(char));
        
        if(strstr(curr_directory, home_directory)!=NULL){
            sprintf(prompt,"~%s", curr_directory+strlen(home_directory));
        }
        else{
            sprintf(prompt,"%s", curr_directory);
        }    
    return prompt;
}