#include <stdio.h>
#include <stdlib.h>

#ifndef DATAMNG
	#define DATAMNG
	#include "datamng.h"
#endif

int start(tlist *,int, char *[]);
int end(tlist *,int,char *[]);
int listall(tlist *,int,char *[]);
int statistic(tlist *,int,char *[]);
int help();
