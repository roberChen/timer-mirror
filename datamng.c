#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

#ifndef DATAMNG
	#define DATAMNG
	#include "datamng.h"
#endif

extern int errno;

mode map[6] = {
	Started,
	Ended,
	Suspend,
	Restarted,
	Plan,
	None
};

char *monmap[12] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug",
		"Sep","Oct","Nov","Dec"
};

char *monmapzh[12] = {
	"一月","二月","三月","四月","五月","六月","七月",
		"八月","九月","十月","十一月","十二月"
};

char *wdaymap[7] = {
	"Monday","Tuesday","Wednsday","Thursday","Friday",
	"Statuaday","Sunday"
};

char *wdaymapzh[7] = {
	"周一","周二","周三","周四","周五","周六","周日"
};

tlist *listinit(){
	tlist *this = calloc(1,sizeof(tlist));
	this->head = NULL;
	this->tail = NULL;
	this->lenth = 0;
	return this;
}

tele *add(tlist *list,char *name, mode status){
	mode newstatus = status;
	if(findbyname(list,name)!=NULL){
		newstatus = Restarted;
	}

	tele *new = calloc(1, sizeof(tele));
	if(new==NULL){
		perror("Creat new activity failed.");
		exit(1);
	}
	if(list->lenth==0){
		list->head = list->tail = new;
		new->prev = NULL;
	}else {
		list->tail->next = new;
		new->prev = list->tail;
		list->tail = new;
	}
	list->lenth++;


	new->name = malloc(strlen(name)+1);
	strcpy(new->name,name);
	new->id = list->lenth;
	new->next = NULL;
	new->status = status;
	new->list = list;
	new->start = 0;
	new->endt = 0;
	new->spndnextid = -1;
	new->status = newstatus;


	if(new->status == Restarted){
		int nextid = findbyname(list,name)->id;
		iterlist *iter = newiter(list);
		for(tele *ele;ele=liter(iter);){
			if(ele->id==nextid && 
				ele->status==Suspend &&
				ele->spndnextid!=-1)
				nextid = ele->spndnextid;
			else if(ele->spndnextid==-1 && ele->id ==nextid){
				ele->status = Suspend;
				ele->spndnextid = new->id;
			}
		}
	}

	return new;
}

int delele(tlist *list, tele *ele){
	if(ele->list != list){
		fprintf(stderr,"tele %p not in tlist %p\n",list, ele);
		return 0;
	}
	if(ele==list->head){
		list->head = ele->next;
		if(list->head!=NULL)
			list->head->prev=NULL;
	}else if(ele==list->tail){
		list->tail = ele->next;
		list->tail->next = NULL;
	}else{
		ele->prev->next = ele->next;
		ele->next->prev = ele->prev;
	}
	free(ele->name);
	free(ele);
	list->lenth--;
	return 1;
}

int ptele(tele *ele,char *fmt){
	//print the ele with format:
	//the format note:
	//	%n :name
	//	%i :id
	//	%s2y %s4y: year of start
	//	%snM %scM %szM: month of start, n means number,
	//					c means characters, z means chinese.
	//	%sd : date of start
	//	%snD %scD %szD : week day of start(Mon, Tue,etc)
	//	%shh %sHh: hour of start.
	//	%sHH : AM or PM of start
	//	%smm :minutes of start
	//	%sss: seconds of start
	//	%e- :sames to s. shows the time of end.
	//	%m : status
	char *cp = fmt;
	struct tm *thistm;
	time_t *t;
	while(*cp){
		if(*cp!='%'){
			putchar(*cp++);
			continue;
		}
		cp++;
		if(*cp=='n'){
			printf("%-*.*s",15,15,ele->name);
			cp++;
			continue;
		}
		if(*cp=='i'){
			printf("%d",ele->id);
			cp++;
			continue;
		}
		if(*cp=='m'){
			char *stname;
			switch(ele->status){
				case Started:
					stname = "Started";
					break;
				case Ended:
					stname = "Ended";
					break;
				case Suspend:
					stname = "Suspend";
					break;
				case Restarted:
					stname = "Restarted";
					break;
				case Plan:
					stname = "Plan";
					break;
				case None:
					stname = "-";
					break;
			}
			printf("%s",stname);
			cp++;
		}
		if(*cp=='s' || *cp=='e'){
			t = *cp=='s'?&ele->start:&ele->endt;
			thistm = localtime(t);
			cp++;
			if(*t==0){
				printf("--");
				cp+=2;
				continue;
			}
			if(strncmp("2y",cp,2)==0){
				printf("%d",thistm->tm_year-(thistm->tm_year/100)*100);
			}else if(strncmp("4y",cp,2)==0){
				printf("%d",thistm->tm_year+1900);
			}else if(strncmp("nM",cp,2)==0){
				printf("%d",thistm->tm_mon);
			}else if(strncmp("cM",cp,2)==0){
				printf("%s",monmap[thistm->tm_mon-1]);
			}else if(strncmp("zM",cp,2)==0){
				printf("%s",monmapzh[thistm->tm_mon-1]);
			}else if(strncmp("hh",cp,2)==0){
				printf("%.2d",thistm->tm_hour>12?thistm->tm_hour-12:thistm->tm_hour);
			}else if(strncmp("Hh",cp,2)==0){
				printf("%.2d",thistm->tm_hour);
			}else if(strncmp("HH",cp,2)==0){
				printf("%s",thistm->tm_hour>12?"PM":"AM");
			}else if(strncmp("dd",cp,2)==0){
				printf("%d",thistm->tm_mday);
			}else if(strncmp("nD",cp,2)==0){
				printf("%d",thistm->tm_wday);
			}else if(strncmp("cD",cp,2)==0){
				printf("%s",wdaymap[thistm->tm_wday-1]);
			}else if(strncmp("zD",cp,2)==0){
				printf("%s",wdaymapzh[thistm->tm_wday-1]);
			}else if(strncmp("mm",cp,2)==0){
				printf("%.2d",thistm->tm_min);
			}else if(strncmp("ss",cp,2)==0){
				printf("%.2d",thistm->tm_sec);
			}
			cp+=2;
			continue;
		}
	}
	return 0;
}

tele *findbyid(tlist *list, int id){
	tele *obj;
	for(iterlist *iter=newiter(list);obj=liter(iter);){
		if(obj!=NULL && obj->id==id)
			return obj;
	}
	return NULL;
}

tele *findbyname(tlist *list,char *name){
	tele *obj;
	for(iterlist *iter=newiter(list);obj=liter(iter);)
		if(obj!=NULL && strcmp(obj->name,name)==0)
			return obj;
	return NULL;
}

int datach(tele *obj,char *fmt,...){
	int count = 0;
	va_list ap;
	va_start(ap,fmt);
	char *cp = fmt;
	while(*cp){
		if(*cp!='%'){
			cp++;
			continue;
		}
		cp++; // to next element;
		switch(*cp){
			case 'n':
				obj->name = va_arg(ap,char *);
				count++;
				break;
			case 'i':
				obj->id = va_arg(ap,int);
				count++;
				break;
			case 'S':
				obj->start = va_arg(ap,time_t);
				count++;
				break;
			case 's':
				obj->start = time(NULL);
				count++;
				break;
			case 'e':
				obj->endt = time(NULL);
				count++;
				break;
			case 'E':
				obj->endt = va_arg(ap,time_t);
				count++;
				break;
			case 'm':
				obj->status = va_arg(ap,mode);
				count++;
				break;
		}
		cp++;
	}
	va_end(ap);
	return count;
}

iterlist *newiter(tlist *obj){
	iterlist *new = calloc(1,sizeof(iterlist));
	new->iterobj = obj;
	new->id = 1;
	new->lenth = obj->lenth;
	new->telep = obj->head;
	//printf("New iterobj with lenth %d.\n",new->lenth);
	return new;
}

tele *liter(iterlist *iterobj){
	if(iterobj->id > iterobj->lenth || iterobj->telep==NULL){
		free(iterobj);
	//	printf("End of iter object.\n");
		return NULL;
	}
	tele *yield = iterobj->telep;
	iterobj->telep = iterobj->telep->next;
	iterobj->id++;
	//printf("ele %p\tid %d\tname %s\n",yield,yield->id,yield->name);
	return yield;
}

tlist *floads(char *fname){
	FILE *fp;
	if((fp=fopen(fname,"r"))==NULL){
		perror("Cannot load data");
		return NULL;
	}
	//loads
	tlist *new = listinit();
	tele *elep;
	while(!feof(fp)){
		elep = calloc(1,sizeof(tele));
		elep->next = NULL;
		elep->list = new;
		elep->name = malloc(101);
		int imode;
		fscanf(fp,"%[^;]%*c %d %d %ld %ld %d\n",
				elep->name,
				&elep->id,
				&elep->spndnextid,
				&elep->start,
				&elep->endt,
				&imode);
		//printf("gets %s;; %d %ld %ld %d\n",elep->name,elep->id,
		//		elep->start,elep->endt,imode);
		elep->status = map[imode];
		if(new->lenth==0){
			new->head = new->tail = elep;
			elep->prev = NULL;
		}else{
			elep->prev = new->tail;
			new->tail->next = elep;
			new->tail = elep;
		}
		new->lenth++;
	}

	fclose(fp);
	//printf("loads %d ele.\n",new->lenth);
	return new;
}

int fsave(tlist *saveobj, char *fname){
	iterlist *iter = newiter(saveobj);
	FILE *fp = fopen(fname,"w");
	int imode;
	for(tele *ele;ele=liter(iter);){
		switch(ele->status){
			case Started:
				imode = 0;break;
			case Ended:
				imode = 1;break;
			case Suspend:
				imode = 2;break;
			case Restarted:
				imode = 3;break;
			case Plan:
				imode = 4;break;
			case None:
				imode = 5;break;
		}
		//printf("Saving %s %d %ld %ld %d at %s\n",
		//		ele->name,
		//		ele->id,
		//		ele->start,
		//		ele->endt,
		//		imode, fname);
		fprintf(fp,"%s; %d %d %ld %ld %d\n",
				ele->name,
				ele->id,
				ele->spndnextid,
				ele->start,
				ele->endt,
				imode);
	}
	fclose(fp);
	return 0;
}
