
#include <stdio.h>
#include <string.h>

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

#define GMP_IN_FILE "..\\..\\gmp-h.in"
#define MPIR_IN_FILE "..\\..\\mpir-h.in"
#define MPIR_OUT_FILE "..\\..\\mpir.h"
#define GMP_OUT_FILE "..\\..\\gmp.h"

int gen_file(char *in_file, char *out_file)
{   FILE *fin, *fout;
    char buf[1000];
    int first = 0;

    if(fin = fopen(in_file, "r"))
    {
        if(fout = fopen(out_file, "w"))
        {
            while(fgets(buf, 1000, fin))
                if(!strncmp(buf, "#if ! defined (__GMP_WITHIN_CONFIGURE)", 38))
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

int main()
{
    gen_file(GMP_IN_FILE, MPIR_OUT_FILE);
    return 0;
}
