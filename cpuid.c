/*

Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
Free Software Foundation, Inc.

Copyright 2008 William Hart.

Copyright 2009,2010,2011 Jason Moxham

Copyright 2010 Gonzalo Tornaria

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

// this should return the microarchitecture , NOT which code path we think is best

#define WANT_FAKE_CPU		0
#define FAKE_CPU		any
#define	FAKE_CPU_VENDOR		"AuthenticAMD"
#define	FAKE_CPU_FAMILY		15
#define FAKE_CPU_EXTFAMILY	0
#define FAKE_CPU_MODEL		12
#define FAKE_CPU_EXTMODEL	0

long fake_cpuid(char *p,unsigned int level);

#if WANT_FAKE_CPU 
#define __gmpn_cpuid fake_cpuid
#else
#if CONFIG_GUESS
#define __gmpn_cpuid(_x,_y)	cpuid(_x,_y,1,0,0)
#endif
#endif

#if CONFIG_GUESS
// use's the stringinzing directive  #x   ie #x expands to "x"
#define CPUIS(x)	modelstr=#x
char*	__gmpn_cpu(int *vector){
#endif
#if INFAT
#define CPUIS(x)	do{TRACE(printf("  "#x"\n"));CPUSETUP_##x;}while(0)
char*	__gmpn_cpu(struct cpuvec_t *vector){
struct cpuvec_t decided_cpuvec;
#endif
  char vendor_string[13];
  char features[12];
  long fms;
  int family, model, stepping,feat;
  char *modelstr=0;

#if INFAT
memset (&decided_cpuvec, '\0', sizeof (decided_cpuvec));
#if FAT32
CPUVEC_SETUP_x86;
#endif
#if FAT64
CPUVEC_SETUP_x86_64;
#endif
CPUVEC_SETUP_fat;
#endif
  __gmpn_cpuid (vendor_string, 0);
  vendor_string[12] = 0;

  fms = __gmpn_cpuid (features, 1);

  family = ((fms >> 8) & 15) + ((fms >> 20) & 0xff);
  model = ((fms >> 4) & 15) + ((fms >> 12) & 0xf0);
  stepping = fms & 15;

  #if CONFIG_GUESS_64BIT
  modelstr = "x86_64";
  #else
  modelstr = "i486";// shouldn't we make this x86??
  #endif

  if (strcmp (vendor_string, "GenuineIntel") == 0)
    {
      switch (family)
	{
	#if CONFIG_GUESS_32BIT || FAT32
	case 5:
	  if (model <= 2) CPUIS(pentium);
	  if (model >= 4) CPUIS(pentiummmx);
	  break;
        #endif
	case 6:
	  #if CONFIG_GUESS_32BIT || FAT32
	  if (model == 1) { CPUIS(pentiumpro);break;}
	  if (model <= 6) { CPUIS(pentium2);break;}
	  if (model <= 13){ CPUIS(pentium3);break;}
	  if (model == 14){ CPUIS(core);break;}
	  #endif
	  if (model == 15){ CPUIS(core2);break;}
	  if (model == 22){ CPUIS(core2);break;}
	  if (model == 23){ CPUIS(penryn);break;}
	  if (model == 26){ CPUIS(nehalem);break;}
	  if (model == 28){ CPUIS(atom);break;}
	  if (model == 29){ CPUIS(penryn);break;}
	  if (model == 30){ CPUIS(nehalem);break;}
	  if (model == 31){ CPUIS(nehalem);break;}
	  if (model == 37){ CPUIS(westmere);break;}
	  if (model == 42){ CPUIS(sandybridge);break;}
	  if (model == 44){ CPUIS(westmere);break;}
	  if (model == 46){ CPUIS(nehalem);break;}
	  if (model == 47){ CPUIS(westmere);break;}
          break;
        case 15:
        #if CONFIG_GUESS_64BIT || FAT64
          __gmpn_cpuid(features,0x80000001);
          if ( features[8]&1 ){ CPUIS(netburstlahf);break;}
          CPUIS(netburst);break;
        #endif
        #if CONFIG_GUESS_32BIT || FAT32
	  if (model <= 6) { CPUIS(pentium4);break;}
	  feat = ((int *)features)[2];
          if (feat & 1) { CPUIS(prescott);break;}
        #endif
          break;
	}
    }
  else if (strcmp (vendor_string, "AuthenticAMD") == 0)
    {
      switch (family)
	{
	#if CONFIG_GUESS_32BIT || FAT32
	case 5:
	  if (model <= 3) { CPUIS(k5);break;}
	  if (model <= 7) { CPUIS(k6);break;}
	  if (model <= 8) { CPUIS(k62);break;}
	  if (model <= 9) { CPUIS(k63);break;}
	  break;
	case 6:
	  CPUIS(k7);
	  break;
        #endif
        case 15:
	  CPUIS(k8);
	  break;
	case 16:
	  if (model == 2) { CPUIS(k10);break;}
	  if (model == 4) { CPUIS(k102);break;}
	  if (model == 5) { CPUIS(k102);break;}
	  if (model == 6) { CPUIS(k102);break;}
	  if (model == 8) { CPUIS(k102);break;}
	  if (model == 9) { CPUIS(k102);break;}
	  if (model == 10) { CPUIS(k102);break;}
	  break;
        case 17:
          CPUIS(k8);// fusion of K8 and GPU
          break;
        case 18:
          CPUIS(k103);// like k102 but with hardware divider
          break;
        case 20:
          CPUIS(bobcat);// fusion of bobcat and GPU
          break;
        // case 21 is bulldozer , or at least some people think so....
        }
    }
  else if (strcmp (vendor_string, "CentaurHauls") == 0)
    {
      switch (family)
	{
	case 6:
	  if (model == 15){CPUIS(nano);break;}
	#if CONFIG_GUESS_32BIT || FAT32
	  if (model < 9) { CPUIS(viac3);break;}
	  CPUIS(viac32);break;
        #endif
	}
    }

#if INFAT
*vector=decided_cpuvec;
#endif
return modelstr;}

long fake_cpuid(char *p,unsigned int level)
{unsigned int eax,feat801=0,feat2=0,family,extfamily,model,extmodel;
 char *vendor;

/*
#if FAKE_CPU == pentium
vendor="GenuineIntel";family= 5;extfamily= 0;model= 2;extmodel= 0;
#endif
#if FAKE_CPU == pentiummmx
vendor="GenuineIntel";family= 5;extfamily= 0;model= 4;extmodel= 0;
#endif
#if FAKE_CPU == pentiumpro
vendor="GenuineIntel";family= 6;extfamily= 0;model= 1;extmodel= 0;
#endif
#if FAKE_CPU == pentium2
vendor="GenuineIntel";family= 6;extfamily= 0;model= 6;extmodel= 0;
#endif
#if FAKE_CPU == pentium3
vendor="GenuineIntel";family= 6;extfamily= 0;model=13;extmodel= 0;
#endif
#if FAKE_CPU == core
vendor="GenuineIntel";family= 6;extfamily= 0;model=14;extmodel= 0;
#endif
#if FAKE_CPU == core2
vendor="GenuineIntel";family= 6;extfamily= 0;model=15;extmodel= 0;
#endif
#if FAKE_CPU == penryn
vendor="GenuineIntel";family= 6;extfamily= 0;model= 7;extmodel= 1;
#endif
#if FAKE_CPU == nehalem
vendor="GenuineIntel";family= 6;extfamily= 0;model=10;extmodel= 1;
#endif
#if FAKE_CPU == atom
vendor="GenuineIntel";family= 6;extfamily= 0;model=12;extmodel= 1;
#endif
#if FAKE_CPU == westmere
vendor="GenuineIntel";family= 6;extfamily= 0;model= 5;extmodel= 2;
#endif
#if FAKE_CPU == sandybridge
vendor="GenuineIntel";family= 6;extfamily= 0;model=10;extmodel= 2;
#endif
#if FAKE_CPU == sandybridge
vendor="GenuineIntel";family= 6;extfamily= 0;model=10;extmodel= 2;
#endif
#if FAKE_CPU == netburst
vendor="GenuineIntel";family=15;extfamily= 0;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == netburstlahf
vendor="GenuineIntel";family=15;extfamily= 0;model= 0;extmodel= 0;feat801=1;
#endif
#if FAKE_CPU == pentium4
vendor="GenuineIntel";family=15;extfamily= 0;model= 6;extmodel= 0;
#endif
#if FAKE_CPU == prescott
vendor="GenuineIntel";family=15;extfamily= 0;model= 7;extmodel= 0;feat2=0x100;
#endif
#if FAKE_CPU == k5
vendor="AuthenticAMD";family= 5;extfamily= 0;model= 3;extmodel= 0;
#endif
#if FAKE_CPU == k6
vendor="AuthenticAMD";family= 5;extfamily= 0;model= 7;extmodel= 0;
#endif
#if FAKE_CPU == k62
vendor="AuthenticAMD";family= 5;extfamily= 0;model= 8;extmodel= 0;
#endif
#if FAKE_CPU == k63
vendor="AuthenticAMD";family= 5;extfamily= 0;model= 9;extmodel= 0;
#endif
#if FAKE_CPU == k7
vendor="AuthenticAMD";family= 6;extfamily= 0;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == k8
vendor="AuthenticAMD";family=15;extfamily= 0;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == k10
vendor="AuthenticAMD";family= 0;extfamily=16;model= 2;extmodel= 0;
#endif
#if FAKE_CPU == k102
vendor="AuthenticAMD";family= 0;extfamily=16;model= 5;extmodel= 0;
#endif
#if FAKE_CPU == k8plus
vendor="AuthenticAMD";family= 1;extfamily=16;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == k103
vendor="AuthenticAMD";family= 2;extfamily=16;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == bobcat
vendor="AuthenticAMD";family= 4;extfamily=16;model= 0;extmodel= 0;
#endif
#if FAKE_CPU == nano
vendor="CentaurHauls";family= 6;extfamily= 0;model=15;extmodel= 0;
#endif
#if FAKE_CPU == vaic3
vendor="CentaurHauls";family= 6;extfamily= 0;model= 8;extmodel= 0;
#endif
#if FAKE_CPU == vaic32
vendor="CentaurHauls";family= 6;extfamily= 0;model= 9;extmodel= 0;
#endif
*/
#if 1
//FAKE_CPU == any
vendor=FAKE_CPU_VENDOR;
family=FAKE_CPU_FAMILY;
extfamily=FAKE_CPU_EXTFAMILY;
model=FAKE_CPU_MODEL;
extmodel=FAKE_CPU_EXTMODEL;
#endif
memset(p,0,12);
if(level==0){strncpy(p,vendor,12);return 1;}
if(level==1){eax=0+(model<<4)+(family<<8)+(0<<12)+(extmodel<<16)+(extfamily<<20);memcpy(p,&feat2,4);return eax;}
if(level==0x80000000){return 1;}
if(level==0x80000001){memcpy(p+8,&feat801,4);return 0;}
return 0;}
