#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef DATAMNG
	#include "datamng.h"
	#define DATAMNG
#endif

#ifndef TSTAT
	#include "tstat.h"
	#define TSTAT
#endif


int main(int argc, char *argv[]){
	printf("Start test.\n");
	tlist *list;
	if((list=floads("save1.list"))==NULL)
		list = listinit();
	else
		printf("Loads form file.\n");
	printf("Lenth of list %d\n",list->lenth);
	char name[20];
	printf("Add new activity:");
	scanf("%[^\n]",name);
	tele *ep = add(list,name,None);
	if(findbyid(list,ep->id)==ep)
		printf("findbyid passed!\n");
	else
		printf("findbyid not passed!\n");
	if(findbyname(list,name)==ep)
		printf("findbyname passed!\n");
	else
		printf("findbyname not passed!\n");
	datach(ep,"%s");
	ptele(ep,"Starts at %sHh:%smm:%sss %s2y\t%emm:%ess\t%m\n");
	sleep(1);
	datach(ep,"%e");
	ptele(ep,"Ends at %eHh:%emm:%ess %e2y or %e4y\n");
	printf("\n\n\nstart statistic\n");
	pall(list,NULL,0);
	iterlist *iter=newiter(list);
	pttitle("\n\n%n\t%i\tperiod\n");
	for(tele *ele;ele=liter(iter);){
		if(ele->spndnextid==-1){
			ptele(ele,"%n\t%i\t");
			printf("%ds\n",getprd(list,ele->name));
		}
	}
	fsave(list,"save1.list");
	printf("End test.\n");
	return 0;
}
