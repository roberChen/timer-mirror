#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _COLOR_H_
#define _COLOR_H_

#define NONE                  "\e[0m"           //normal
#define BLACK                 "\e[0;30m"         //dark
#define L_BLACK             "\e[1;30m"            //bright dark
#define RED                     "\e[0;31m"      //deep red
#define L_RED                 "\e[1;31m"        //bright red
#define GREEN                "\e[0;32m"         //deep green
#define L_GREEN            "\e[1;32m"           //bright green
#define BROWN               "\e[0;33m"          //deep yellow
#define YELLOW              "\e[1;33m"          //bright yellow
#define BLUE                    "\e[0;34m"      //deep blue
#define L_BLUE                "\e[1;34m"        //bright blue
#define PURPLE               "\e[0;35m"         //deep pink
#define L_PURPLE           "\e[1;35m"           //bright pink
#define CYAN                   "\e[0;36m"       //deep green
#define L_CYAN               "\e[1;36m"         //bright green
#define GRAY                   "\e[0;37m"       //gray
#define WHITE                  "\e[1;37m"       //white
#define BOLD                    "\e[1m"         //bold white
#define UNDERLINE         "\e[4m"               //underline white
#define BLINK                   "\e[5m"         //blink white
#define REVERSE            "\e[7m"              //reverse
#define HIDE                     "\e[8m"        //hide
#define CLEAR                  "\e[2J"          //clear
#define CLRLINE               "\r\e[K"          //clear line

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
		*cp++=' ';
	}
	*--cp='\0';
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
	argc = optloads(optlist,&others,argc,argv);
	if(ifid&&ifname){
		printf("Argument error: id and name options cannot occurs at the same time.\n");
		exit(2);
	}else if(!(ifid||ifname)){
		printf("Please specify your activity locate method:\n"
				"\t-i for locating by id.\n"
				"\t-n for locating by name\n");
		exit(2);
	}
	if(ifid){
		ele = findbyid(list,atoi(others[0]));
		datach(ele,"%e%m",Ended);
	}else if(ifname){
		char name[500];
		char *cp = name;
		while(--argc){
			(*others)++;
			for(;*cp=*(*others);cp++,(*others)++);
			*cp++=' ';
		}
		*--cp='\0';
		
		if(!findbyname(list, name)){
			fprintf(stderr,"End failed. Not activity named \"%s\".\n",others[0]);
			return -1;
		}
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
	if(!ele)
		return -1;
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

int help(){
	printf(
			"Usage:\n"
			"\t"BOLD"start <name>"NONE" \tto start a activity.\n"
			"\t"BOLD"end -i <id>"NONE" or "BOLD"end -n <name>"NONE" \tto end an activity.\n"
			"\t"BOLD"list"NONE" \tto list all activities.\n"
			"\t"BOLD"statistic"NONE" \t to statistic all activities.\n"
			);
	return 0;
}
