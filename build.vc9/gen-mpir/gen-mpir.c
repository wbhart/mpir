
#include <stdio.h>

char *add = 
"#if ! defined (__GMP_WITHIN_CONFIGURE)\n"
"#  if defined( _MSC_VER )\n"
"#    if defined( _WIN64 )\n"
"#        define SIZEOF_MP_LIMB_T           8\n"
"#        define _LONG_LONG_LIMB            1\n"
"#    elif defined( _WIN32 )\n"
"#        define SIZEOF_MP_LIMB_T           4\n"
"#        ifdef _LONG_LONG_LIMB\n"
"#          undef _LONG_LONG_LIMB\n"
"#        endif\n"
"#    else\n"
"#        error This is the wrong version of mpir.h\n"
"#    endif\n"
"#  endif\n"
"#  define GMP_LIMB_BITS            (SIZEOF_MP_LIMB_T << 3)\n"
"#  define __GMP_BITS_PER_MP_LIMB   (SIZEOF_MP_LIMB_T << 3)\n"
"#  define GMP_NAIL_BITS            0\n"
"#endif\n";

#define IN_FILE "..\\..\\gmp-h.in"
#define OUT_FILE "..\\..\\mpir.h"

int main()
{   FILE *fin, *fout;
    char buf[1000];
    int first = 0;

    if(fin = fopen(IN_FILE, "r"))
    {
        if(fout = fopen(OUT_FILE, "w"))
        {
            while(fgets(buf, 1000, fin))
                if(!strncmp(buf, "/* Instantiated by configure. */", 32))
                {
                    while(fgets(buf, 1000, fin))
                    {
                        if(!strncmp(buf, "#endif", 6))
                        {
                            if(!first)
                                fputs(add, fout); 
                            first = 1;
                            break;
                        }
                    }
                }
                else
                    fputs(buf, fout);
            fclose(fout);
        }
        fclose(fin);
    }

    return 0;
}