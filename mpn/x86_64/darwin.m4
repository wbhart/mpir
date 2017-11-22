divert(-1)

dnl  m4 macro overrrides for Darwin x86-64 assembler.

define(`JMPENT',`dnl
ifdef(`PIC',
    `.set   $1_tmp, $1-$2
    .long   $1_tmp'
,
    `.quad  $1'
)')

divert`'dnl
