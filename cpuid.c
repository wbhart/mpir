// this should return the microarchitecture , NOT which code path we think is best

#if CONFIG_GUESS
// use's the stringinzing directive  #x   so MODELSTR(teddy)  expands to modelstr="teddy"
#define CPUIS(x)	modelstr=#x
#define __gmpn_cpuid	cpuid
#endif
#if INFAT
#define CPUIS(x)	do{TRACE(printf("  "#x"\n"));CPUSETUP_##x;}while(0)
#endif

  char vendor_string[13];
  char features[12];
  long fms;
  int family, model, stepping;
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
	  break;
        case 15:
        #if CONFIG_GUESS_64BIT || FAT64
          __gmpn_cpuid(features,0x80000001);
          if ( features[8]&1 ){ CPUIS(netburstlahf);break;}
          CPUIS(netburst);
        #endif
        #if CONFIG_GUESS_32BIT || FAT32
	  if (model <= 6) { CPUIS(pentium4);break;}
	  int feat = ((int *)features)[2];
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
	  if (model == 2) { CPUIS(k10);break;} // phenom
	  if (model == 4) { CPUIS(k10);break;} //phenom II
	  break;
        }
    }
  #if CONFIG_GUESS_32 || FAT32
  else if (strcmp (vendor_string, "CyrixInstead") == 0)
    {
      /* Should recognize Cyrix' processors too.  */
    }
  else if (strcmp (vendor_string, "CentaurHauls") == 0)
    {
      switch (family)
	{
	case 6:
	  if (model < 9) { CPUIS(viac3);break;}
	  CPUIS(viac32);break;
	}
    }
  #endif
