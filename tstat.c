#include <stdio.h>
#include <stdlib.h>

#ifndef DATAMNG
	#include "datamng.h"
	#define DATAMNG
#endif

#ifndef TSTAT
	#include "tstat.h"
	#define TSTAT
#endif



int pall(tlist *list,char *fmt,int iftotal){
	int count=0;
	if(fmt==NULL){
		fmt = "%n\t%i\t%sHh:%smm\t%eHh:%emm\t%m\n";
	}
	pttitle(fmt);
	iterlist *iter = newiter(list);
	for(tele *ele;ele=liter(iter);count++){
		ptele(ele,fmt);
	}
	if(iftotal)
		printf("Total:%d\n",count);
	return count;
}


int pttitle(char *fmt){
	char *cp = fmt;
	while(*cp){
		if(*cp!='%'){
			putchar(*cp++);
			continue;
		}
		cp++;
		if(*cp=='n'){
			printf("%-*.*s",15,15,"Name");
			cp++;
			continue;
		}
		if(*cp=='i'){
			printf("Id");
			cp++;
			continue;
		}
		if(*cp=='m'){
			printf("Status");
			cp++;
		}
		if(*cp=='s' || *cp=='e'){
			printf("%c",*cp);
			cp++;
			if(strncmp("2y",cp,2)==0){
				printf("yr");
			}else if(strncmp("4y",cp,2)==0){
				printf("year");
			}else if(strncmp("nM",cp,2)==0 ||
					strncmp("cM",cp,2)==0 ||
					strncmp("zM",cp,2)==0){
				printf("Mn");
			}else if(strncmp("hh",cp,2)==0 ||
					strncmp("Hh",cp,2)==0){
				printf("Hr");
			}else if(strncmp("HH",cp,2)==0){
				printf("  ");
			}else if(strncmp("dd",cp,2)==0){
				printf("dt");
			}else if(strncmp("nD",cp,2)==0 ||
					strncmp("cD",cp,2)==0 ||
					strncmp("zD",cp,2)==0){
				printf("wd");
			}else if(strncmp("mm",cp,2)==0){
				printf("mn");
			}else if(strncmp("ss",cp,2)==0){
				printf("sc");
			}
			cp+=2;
			continue;
		}
	}
	return 0;
}


int getprd(tlist *list,char *name){
	int difft=0;
	int nextid = findbyname(list,name)->id;
	iterlist *iter=newiter(list);
	for(tele *ele;ele=liter(iter);){
		if(ele->id==nextid){
			if(ele->endt==0)
				difft+=(time(NULL) - ele->start);
			else
				difft+=ele->endt - ele->start;
			if(ele->spndnextid != -1){
				nextid = ele->spndnextid;
			}else
				break;
		}
	}
	return difft;
}

char *sgethprd(int sprd){
	char *str = malloc(11);
	int d,h,m,s;
	d = sprd/(3600*24);
	h = (sprd-d*3600*24)/3600;
	m = (sprd-d*3600*24-h*3600)/60;
	s = sprd - d*3600*24 - h*3600 - m*60;
	if(d==0)
		sprintf(str,"%d:%.2d:%.2d",h,m,s);
	else
		sprintf(str,"%d:%d:%.2d:%.2d",d,h,m,s);

	return str;
}


char *srecon(char *origion){
	char *new = malloc(strlen(origion)+1);
	char *np = new,*op=origion;
	for(;*op;){
		if(*op!='\\')
			*np++ = *op++;
		else if(*op=='\\'){
			if(strncmp("\\n",op,2)==0){
				*np='\n';
				np++;
				op+=2;
			}else if(strncmp("\\t",op,2)==0){
				*np='\t';
				np++;
				op+=2;
			}
		}
	}

	return new;
}
