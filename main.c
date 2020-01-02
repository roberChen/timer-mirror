#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UI
	#define UI
	#include "ui.h"
#endif

#ifndef DATAMNG
	#define DATAMNG
	#include "datamng.h"
#endif

#ifndef TSTAT
	#define TSTAT
	#include "tstat.h"
#endif

#ifndef TOPT
	#include "topts.h"
	#define TOPT
#endif

int main(int argc, char *argv[]){
	if(argc==1){
		printf("Missing command!\n");
		exit(1);
	}
	//init
	tlist *list;
	if((list=floads("save1.list"))==NULL)
		list = listinit();

	argc--;
	argv++;
	if(strcmp("start",*argv)==0){
		start(list,argc,argv);
	}else if(strcmp("end",*argv)==0){
		end(list,argc,argv);
	}else if(strcmp("list",*argv)==0){
		listall(list,argc,argv);
	}else if(strcmp("statistic",*argv)==0){
		statistic(list,argc,argv);
	}

	//end
	fsave(list,"save1.list");

	return 0;
}
