#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "addcc	%r4,%5,%1\n"						\
      "	addccc	%r6,%7,%%g0\n"						\
      "	addc	%r2,%3,%0"						\
	  : "=r" (sh), "=&r" (sl)					\
	  : "rJ" (ah), "rI" (bh), "%rJ" (al), "rI" (bl),		\
	    "%rJ" ((al) >> 32), "rI" ((bl) >> 32)			\
	   __CLOBBER_CC)
#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "subcc	%r4,%5,%1\n"						\
      "	subccc	%r6,%7,%%g0\n"						\
      "	subc	%r2,%3,%0"						\
	  : "=r" (sh), "=&r" (sl)					\
	  : "rJ" (ah), "rI" (bh), "rJ" (al), "rI" (bl),		\
	    "rJ" ((al) >> 32), "rI" ((bl) >> 32)			\
	   __CLOBBER_CC)
