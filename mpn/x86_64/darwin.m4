define(`JMPENT',`dnl
ifdef(`PIC',
    `.set   $1_tmp, $1-$2
    .long   $1_tmp'
,
    `.quad  $1'
)')
