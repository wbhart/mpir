/*

Copyright 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006
Free Software Foundation, Inc.

Copyright 2008 William Hart.

Copyright 2009 Jason Moxham

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

#if CONFIG_GUESS
// use's the stringinzing directive  #x   ie #x expands to "x"
#define CPUIS(x)	modelstr=#x
#define __gmpn_cpuid(_x,_y)	cpuid(_x,_y,1,0,0)
#endif
#if INFAT
#define CPUIS(x)	do{TRACE(printf("  "#x"\n"));CPUSETUP_##x;}while(0)
#endif

  char vendor_string[13];
  char features[12];
  long fms;
  int family, model, stepping,feat;
  char *modelstr;

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
          CPUIS(k102);// like k102 but with hardware divider
          break;
        case 20:
          CPUIS(k8);// fusion of bobcat and GPU
          break;
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

