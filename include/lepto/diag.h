#ifndef LEPTO_DIAG_H
#define LEPTO_DIAG_H
/**---------------------------------------------------------------------------
 *
 * @file    diag.h
 * @brief   Optional diagnose
 *
 *          The macros can be used to optiopnally track statistics. They can
 *          be disabled in final release.
 *
 * @date      20250916
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdarg.h>
#include <lepto/lepto.h>               // IS_ENABLED( )


/*--- Implementation -------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_DIAG )

#define DIAG_DEF(a) int m_diag_ ## a = 0;
#define DIAG_INC(a) { m_diag_ ## a ++; }
#define DIAG_VAL(a) m_diag_ ## a
#define DIAG_MAX(a, b) { if( b > m_diag_ ## a ) m_diag_ ## a = b; }
#define DIAG_DUMP(a) { printf("Diag/" #a ":%d\n", DIAG_VAL( a ) );}

#else

#define DIAG_DEF(a)
#define DIAG_INC(a)
#define DIAG_VAL(a) 0
#define DIAG_MAX(a, b)
#define DIAG_DUMP(a)

#endif // ? CONFIG_LEPTO_DIAG


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_DIAG_H
