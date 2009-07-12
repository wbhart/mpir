long cpuid(char *p,int i)
{int a[4];

__cpuid(a,i);
memcpy(p,&(a[1]),4);
memcpy(p+4,&(a[3]),4);
memcpy(p+8,&(a[2]),4);
return a[0];}

main (int argc,char *argv[])
{char *p;int a=sizeof(p)*8;

#define CONFIG_GUESS		1
// safe to always try 32bit
#define CONFIG_GUESS_32BIT	1
#define CONFIG_GUESS_64BIT	0
#define FAT32			0
#define FAT64			0
#define INFAT			0
#include "../cpuid.c"
if(argc==2){printf("set GCPU=%s\nset GBITS=%d\n",modelstr,a);return 0;}
printf("%s-pc-Win%d\n",modelstr,a);return 0;}
