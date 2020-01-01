# Reference of datamng

[TOC]

## description of datamng

datamng is a C library for timer. It manage all the timer activities, including adding, editing, deleting, saving, loading, printing.

## Library reference

The C library defines following types

```c
typedef enum{
	Started,
	Ended,
	Suspend,
	Plan,
	None
} mode;
//defines the mode of each activities

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
//defines the activity element

struct strTlist{
	int lenth;
	tele *head;
	tele *tail;
};
//defines the list object

typedef struct{
	tlist *iterobj;
	int id;
	int lenth;
	tele *telep;
} iterlist;
//defines the iter-object for tlist

```



The C library has following functions

```c  
tlist *listinit();//creating a tlist object
tele *add(tlist *list, char *name, mode status);//add a new tele element in tlist
int delele(tlist *list, tele *ele);//deleat a tele object
int ptele(tele *ele,char *fmt);//print a tele with format defines in fmt
tele *findbyid(tlist *list, int id);//find tele in tlist by id
tele *findbyname(tlist *list, char *name);//find tele in tlist by name
int datach(tele *obj, char *fmt,...);//modify a tele, with sequece defines in fmt
iterlist *newiter(tlist *obj);//creat a init object for tlist
tele *liter(iterlist *iterobj);//iter the iter-object
tlist *floads(char *fname);//loading a tlist from file named fname
int fsave(tlist *saveobj, char *fname);//saving a tlist to a file named fname;

```

## Functions usage

**listinit()**

Returns a pointer to an empty tlist object.

**add(tlist *list, char *name, mode status)**

Add a tele object to the end of tlist, which name defines by *name*, and mode status *status*.

The Status are:

- Started: the activity is on process.
- Ended : the activity has stopped.
- Suspend: the activity is suspended and might restart later
- Plan: the activity is still a plan and has not been started
- None: None mode.

the functions returns the pointer to the newly added tele object.

**delele(tlist *list, tele *ele)**

Delete the tele object in tlist. Returns success or not

**ptele(tele *ele, char *fmt)**

Print tele to *stdout* with fmt:

|               note               | descriptions                                                 |
| :------------------------------: | ------------------------------------------------------------ |
|             ```%n```             | name                                                         |
|             ```%i```             | id                                                           |
|            ```%s-```             | Print the infos of start time, ```-``` are the following parameters |
|            ```%e-```             | Print the infos of end time.                                 |
|       ```2y```*or*```4y```       | year, ```2y```print the year with 2 numbers while ```4y```print 4. For example 19 or 2019. |
| ```nM```*or*```cM```*or*```zM``` | month in number, English characters or Chinese characters. For example 2 or Feb or 二月 |
|             ```dd```             | date.                                                        |
| ```nD```*or*```cD```*or*```zD``` | week day. eg 4 *or* Thursday *or* 周四                       |
|       ```hh```*or*```Hh```       | print the hour.                                              |
|             ```HH```             | print AM or PM                                               |
|             ```mm```             | print minute                                                 |
|             ```ss```             | print seconds                                                |
|             ```m```              | status of the activity                                       |

Returns success or not

**findbyid(tlist *list, int id)***

Find tele which id is *id* in list, returns the pointer of the tele object.

**findbyname(tlist *list, char *name)***

Find tele which name is *name* in list, returns the pointer of the tele object.

**datach(tele *obj, char *fmt,...)**

The format are follows:

| name    | descriptions                                     |
| ------- | ------------------------------------------------ |
| ```n``` | name                                             |
| ```i``` | id                                               |
| ```s``` | change the start time as current time            |
| ```S``` | change the start time with given time_t argument |
| ```e``` | change the end time as current time              |
| ```E``` | change the end time with given time_t argument   |
| ```m``` | change the mode with given mode argument         |

