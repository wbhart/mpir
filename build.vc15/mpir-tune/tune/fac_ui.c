#define TUNE_PROGRAM_BUILD 1
#define __gmpz_fac_ui mpz_fac_ui_tune
#define __gmpz_oddfac_1 mpz_oddfac_1_tune
#include "..\..\..\mpz\oddfac_1.c"
#include "..\..\..\mpz\fac_ui.c"
