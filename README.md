# Shell in C 

## Data Structures used along with their associated methods

### StringLinked

Basically a linked list with each node storing a string and a integer.

* strLinkInit
>void strLinkInit(strLink* list);

Initialises the strLink structure given "list"(argument 1), allocating memory for the head and the tail node.

* strLinkAdd
>void strLinkAdd(strLink* list, char *str, int pidNum);
Adds a node with string "str"(argument 2) and integer "pidNum"(argument 3), to the "list"(argument 1) strLink structure.

* strLinkRemove
>void strLinkRemove(strLink* list, strLinkNode *inst);

Removes the particular "inst"(argument 2) node from "list"(argument 1) structure.

* StrFindPid
>char* StrFindPid(strLink *s, int pid);

Finds a node within the given strLink structure "s"(argument 1) where the node member pid = "pid"(argument 2)

### StringVector 

* StringVectorAdd
>void StringVectorAdd(StringVector* LS, char* str);

Given a StringVector it adds a character string given by str to LS, using malloc and function given in "string.h", if the size is limited it increases the size of the pointer array. 

* StringVectorErase
>void StringVectorErase(StringVector* V);

For every pointer in the list of character pointers, it frees up the memory taken by each of them, and also the memory taken by the array of pointers.

* StringVectorInit
>void StringVectorInit(StringVector* LS);

Initializes the data structure.

* StringVectorCopy
>void StringVectorCopy(StringVector *src, StringVector *dst);

Copy one StringVector to another.

* StringVectorReplace
>void StringVectorReplace(StringVector* l, int index, char *str);

Replaces a string stored in the StringVector l with another string.

## Miscellaneous Functions 

### ArgsFinder
> int ArgsFinder(StringVector* l, char *arg)

This function just returns the position of the arg (string) in StringVector l (can be thought of as a array of strings) if it exists else it returns -1.

### CommandHandler
>void CommandHandler(StringVector *l);

Handles bulk of the work, after recieveing the command, after getting tokenized the command is sent to CommandHandler, which handles most of the commands except the "exit" command. 

### lsHandler
>void lsHandler(StringVector *l);

lsHandler handles the ls command.

### p_info
>void p_info(pid_t pid)

p_info displays the status of the process with the process id pid argument.

### pwd
>char* pwd()

Converts any absolute address to relative address if applicable i.e if the directory is a subdirectory of the home directory. 

### prompt
>void prompt()

Handles printing the prompty taking in the input command and then tokenizing it by calling the InputSantize  function, and handles the part of adding commands to history and calling the CommandHandler.

### InputSantize
>int InputSanitize(char* input, StringVector* l, char* delim)

Given a string input and the delimeter to separate the input with, it separates the input into separate string and adds them into the StringVector l.

### 


