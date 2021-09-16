#ifndef __UTIL_H_
#define __UTIL_H_

char *curr_directory;
char *home_directory;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include "unistd.h"
#include "history.h"
#include "StringLinked.h"

#include "StringVector.c"
#include "ArgsFinder.c"
#include "lsHandler.c"
#include "CommandHandler.c"
#include "p_info.c"
#include "history.c"
#include "StringLinked.c"

#endif