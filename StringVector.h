#include "utils.h"

typedef struct _StringVector {
    char **list;
    int size;
    int maxSize;
} StringVector;

void StringVectorAdd(StringVector* LS, char* str);

void StringVectorErase(StringVector* V);

void StringVectorInit(StringVector* LS);

void StringVectorCopy(StringVector *src, StringVector *dst);