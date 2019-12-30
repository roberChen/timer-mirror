#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

#include "datamng.h"

extern int errno;

mode map[5] = {
	Started,
	Ended,
	Suspend,
	Plan,
	None
};

char *monmap[12] = {
	"Jan","Fab","Mar","Apr","May","Jun","Jul","Aug",
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
	//	%s12h %s24h: hour of start.
	//	%sH : AM or PM of start
	//	%sm :minutes of start
	//	%ss: seconds of start
	//	%e- :sames to s. shows the time of end.
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
			printf("%s",ele->name);
			cp++;
			continue;
		}
		if(*cp=='i'){
			printf("%d",ele->id);
			cp++;
			continue;
		}
		if(*cp=='s' || *cp=='e'){
			t = *cp=='s'?&ele->start:&ele->endt;
			thistm = localtime(t);
			cp++;
			if(strncmp("2y",cp,2)==0){
				printf("%d",thistm->tm_year-(thistm->tm_year/100)*100);
				cp+=2;
				continue;
			}else if(strncmp("4y",cp,2)==0){
				printf("%d",thistm->tm_year+1900);
				cp+=2;
				continue;
			}else if(strncmp("nM",cp,2)==0){
				printf("%d",thistm->tm_mon);
				cp+=2;
				continue;
			}else if(strncmp("cM",cp,2)==0){
				printf("%s",monmap[thistm->tm_mon-1]);
				cp+=2;
				continue;
			}else if(strncmp("zM",cp,2)==0){
				printf("%s",monmapzh[thistm->tm_mon-1]);
				cp+=2;
				continue;
			}else if(strncmp("12h",cp,3)==0){
				printf("%.2d",thistm->tm_hour>12?thistm->tm_hour-12:thistm->tm_hour);
				cp+=3;
				continue;
			}else if(strncmp("24h",cp,3)==0){
				printf("%.2d",thistm->tm_hour);
				cp+=3;
				continue;
			}else if(*cp=='H'){
				printf("%s",thistm->tm_hour>12?"PM":"AM");
				cp++;
				continue;
			}else if(*cp=='d'){
				printf("%d",thistm->tm_mday);
				cp++;
				continue;
			}else if(strncmp("nD",cp,2)==0){
				printf("%d",thistm->tm_wday);
				cp+=2;
			}else if(strncmp("cD",cp,2)==0){
				printf("%s",wdaymap[thistm->tm_wday-1]);
				cp+=2;
				continue;
			}else if(strncmp("zD",cp,2)==0){
				printf("%s",wdaymapzh[thistm->tm_wday-1]);
				cp+=2;
				continue;
			}else if(*cp=='m'){
				printf("%.2d",thistm->tm_min);
				cp++;
			}else if(*cp=='s'){
				printf("%.2d",thistm->tm_sec);
				cp++;
			}
		}
	}
	return 0;
}

tele *findbyid(tlist *list, int id){
	tele *obj;
	for(iterlist *iter=newiter(list);obj=liter(iter);){
		if(obj->id==id)
			return obj;
	}
	return NULL;
}

tele *findbyname(tlist *list,char *name){
	tele *obj;
	for(iterlist *iter=newiter(list);obj=liter(iter);)
		if(strcmp(obj->name,name)==0)
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
			case Plan:
				imode = 3;break;
			case None:
				imode = 4;break;
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
