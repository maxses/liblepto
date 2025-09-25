#ifndef LEPTO_UNITS_H
#define LEPTO_UNITS_H
/**---------------------------------------------------------------------------
 *
 * @file    units.h
 * @brief   Helper defines for units
 *
 * @date      20250827
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#define SEC_PER_MIN     60l
#define MIN_PER_HOUR    60l
#define SEC_PER_HOUR    3600l
#define SEC_PER_DAY     ( SEC_PER_HOUR * 24l)
// SEC_PER_YEAR: Without leap years of course, just for approach
#define SEC_PER_YEAR    ( SEC_PER_DAY  * 365l)
#define MSEC_PER_HOUR   ( SEC_PER_HOUR * MSEC_PER_SEC )
#define NSEC_PER_SEC    1000000000l     // nano
#define USEC_PER_SEC    1000000l        // micro
#define USEC_PER_MIN    ( USEC_PER_SEC * SEC_PER_MIN )
#define MSEC_PER_SEC    ( 1000l )           // milli
#define MSEC_PER_MIN    ( MSEC_PER_SEC * SEC_PER_MIN )
#define USEC_PER_MSEC   ( 1000l )
#define SUBSEC_PER_SEC  0x100
#define MSEC_PER_SUBSEC (1000/0x100)

#define BYTES_PER_MB    ( 1024 * 1024 )

#define SECONDS_SINCE_EPOCH_ON_MILLENNIUM       ( 946684800l )
#define DAYS_SINCE_EPOCH_ON_MILLENNIUM          ( 10957l )


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_UNITS_H
