#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>

#include "datamng.h"

extern int errno;

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
	ele->prev->next = ele->next;
	ele->next->prev = ele->prev;
	free(ele->name);
	free(ele);
	list->lenth--;
	return 1;
}

tele *findbyid(tlist *list, int id){
	tele *obj;
	for(obj=list->head;obj->id!=id;obj=obj->next){
		if(obj==NULL)
			return NULL;
	}
	return obj;
}

tele *findbyname(tlist *list,char *name){
	tele *obj;
	for(obj=list->head;strcmp(obj->name,name)!=0;obj=obj->next)
		if(obj==NULL)
			return NULL;
	return obj;
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
	new->id = 0;
	new->lenth = obj->lenth;
	new->telep = obj->head;

	return new;
}

tele *liter(iterlist *iterobj){
	if(iterobj->id == iterobj->lenth){
		free(iterobj);
		return NULL;
	}
	iterobj->telep = iterobj->telep->next;
	iterobj->id++;
	return iterobj->telep;
}

tlist *floads(char *fname){
	FILE *fp;
	if((fp=fopen(fname,"r"))==NULL){
		perror("Cannot load data %s:");
		fclose(fp);
		exit(1);
	}
	//loads
	tlist *new = listinit();

	tele *elep;
	char s[500], *cp;
	while(fscanf(fp,"%s",s)!=EOF){
		elep = calloc(1,sizeof(tele));
		elep->next = NULL;
		elep->list = new;
		for(cp=s;*++cp!=' ';);
		elep->name = malloc(cp-s+1);
		elep->name[cp-s] = '\0';
		strncpy(elep->name,s,cp-s);
		int imode;
		sscanf((const char *)cp,"%d %ld %ld %d",
				&elep->id, 
				&elep->start,
				&elep->endt,
				&imode);
		switch(imode){
			case 0:
				elep->status = Started;break;
			case 1:
				elep->status = Ended;break;
			case 2:
				elep->status = Suspend;break;
			case 3:
				elep->status = Plan;break;
			case 4:
				elep->status = None;break;
		}
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
		fprintf(fp,"%s %d %ld %ld %d",
				ele->name,
				ele->id,
				ele->start,
				ele->endt,
				imode);
	}
	fclose(fp);
	return 0;
}
