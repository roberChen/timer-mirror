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
	char s[500], *cp;
	while(!feof(fp)){
		elep = calloc(1,sizeof(tele));
		elep->next = NULL;
		elep->list = new;
		for(cp=s;*++cp!=' ';);
		elep->name = malloc(cp-s+1);
		int imode;
		fscanf(fp,"%s %d %ld %ld %d\n",elep->name,&elep->id,
						&elep->start,&elep->endt,&imode);
		//printf("gets %s %d %ld %ld %d\n",elep->name,elep->id,
		//		elep->start,elep->endt,imode);
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
		fprintf(fp,"%s %d %ld %ld %d\n",
				ele->name,
				ele->id,
				ele->start,
				ele->endt,
				imode);
	}
	fclose(fp);
	return 0;
}
