#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int start(tlist *list,int argc,char *argv[]){
	char s[500], *cp;
	cp = s;
	if(argc==1){
		printf("Name unknown.\n");
		exit(1);
	}
	while(--argc){
		argv++;
		for(;*cp=*(*argv);cp++,(*argv)++);
		*++cp=' ';
	}
	*cp='\0';
	tele *new = add(list,s,Started);
	datach(new,"%s");
	ptele(new,"Started %n\tid:%i,%sHh:%smm:%sss\tmode:%m\n");
	return 0;
}

int end(tlist *list,int argc,char *argv[]){
	int ifid=0;
	int ifname=0;
	char **others;
	tele *ele;
	Opts *optlist = opts(2,"i|id:",&ifid,"n|name:",&ifname);
	optloads(optlist,&others,argc,argv);
	if(ifid&&ifname){
		printf("Argument error: id and name options cannot occurs at the same time.\n");
		exit(2);
	}
	if(ifid){
		ele = findbyid(list,atoi(others[0]));
		datach(ele,"%e%m",Ended);
	}else if(ifname){
		int nextid = findbyname(list,others[0])->id;
		for(iterlist *iter=newiter(list);
				ele=liter(iter);){
			if(ele->id==nextid&&ele->spndnextid!=-1)
				nextid = ele->spndnextid;
			else if(ele->id==nextid&&ele->spndnextid==-1){
				datach(ele,"%e%m",Ended);
				break;
			}
		}
	}

	ptele(ele,"%n\t%i\t%eHh:%emm:%ess,%m\n");
	return 0;
}

int listall(tlist *list,int argc,char *argv[]){
	char *fmt,**others;
	fmt=NULL;
	int iftotal=0;
	Opts *optlist = opts(2,"f|forms:s",&fmt,"t|total:",&iftotal);
	optloads(optlist,&others,argc,argv);
	pall(list,srecon(fmt),iftotal);
	return 0;
}

int statistic(tlist *list,int argc,char *argv[]){
	iterlist *iter = newiter(list);
	pttitle("%n\t%i\tperiod\n");
	for(tele *ele;ele=liter(iter);){
		if(ele->spndnextid==-1){
			ptele(ele,"%n\t%i\t");
			printf("%s\n",sgethprd(getprd(list,ele->name)));
		}
	}
	return 0;
}
