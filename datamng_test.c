#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "datamng.h"


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
	ptele(ep,"Starts at %s24h:%sm:%ss %s2y or %s4y\n");
	sleep(1);
	datach(ep,"%e");
	ptele(ep,"Ends at %e24h:%em:%es %e2y or %e4y\n");
	fsave(list,"save1.list");
	printf("End test.\n");
	return 0;
}
