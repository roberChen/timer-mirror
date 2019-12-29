#include <stdarg.h>
#include <time.h>
typedef enum{
	Started,
	Ended,
	Suspend,
	Plan,
	None
} mode;

typedef struct strTele tele;
typedef struct strTlist tlist;

struct strTele{
	char *name;
	int id;
	tele *prev;
	tele *next;
	int spndnextid;
	tlist *list;
	time_t start;
	time_t endt;
	mode status;
};

struct strTlist{
	int lenth;
	tele *head;
	tele *tail;
};

typedef struct{
	tlist *iterobj;
	int id;
	int lenth;
	tele *telep;
} iterlist;

tlist *listinit();
tele *add(tlist *list, char *name, mode status);
int delele(tlist *list, tele *ele);
int ptele(tele *,char *);
tele *findbyid(tlist *list, int id);
tele *findbyname(tlist *list, char *name);
int datach(tele *obj, char *fmt,...);
iterlist *newiter(tlist *obj);
tele *liter(iterlist *iterobj);
tlist *floads(char *fname);
int fsave(tlist *saveobj, char *fname);
