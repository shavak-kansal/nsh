#include <stdlib.h>
#include <string.h>

typedef struct _StringVector {
    char **list;
    int size;
    int maxSize;
} StringVector;

void StringVectorAdd(StringVector* LS, char* str);

void StringVectorErase(StringVector* V);