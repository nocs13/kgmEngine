#ifndef KGMEXCEPTION_H
#define KGMEXCEPTION_H

#include <setjmp.h>
#define kgm_try_start do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ) {
#define kgm_except } else {
#define kgm_try_end } }while(0)
#define kgm_throw longjmp(ex_buf__, 1)

#endif // KGMEXCEPTION_H
