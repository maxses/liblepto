#ifndef LEPTO_UNITS_H
#define LEPTO_UNITS_H
/**---------------------------------------------------------------------------
 *
 * @file    units.h
 * @brief   Helper defines for units
 *
 * @date      20250827
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#define SEC_PER_MIN     60ul
#define MIN_PER_HOUR    60ul
#define MIN_PER_DAY     ( MIN_PER_HOUR * 24ul )
#define SEC_PER_HOUR    3600ul
#define SEC_PER_DAY     ( SEC_PER_HOUR * 24ul)
// SEC_PER_YEAR: Without leap years of course, just for approach
#define SEC_PER_YEAR    ( SEC_PER_DAY  * 365ul)
#define MSEC_PER_HOUR   ( SEC_PER_HOUR * MSEC_PER_SEC )
#define NSEC_PER_SEC    1000000000ul     // nano
#define USEC_PER_SEC    1000000ul        // micro
#define USEC_PER_MIN    ( USEC_PER_SEC * SEC_PER_MIN )
#define MSEC_PER_SEC    ( 1000ul )           // milli
#define MSEC_PER_MIN    ( MSEC_PER_SEC * SEC_PER_MIN )
#define USEC_PER_MSEC   ( 1000ul )
#define SUBSEC_PER_SEC  0x100ul
#define MSEC_PER_SUBSEC (1000ul/0x100ul)
#define HOUR_PER_DAY    ( 24ul )

#define BYTES_PER_MB    ( 1024 * 1024 )

#define SECONDS_SINCE_EPOCH_ON_MILLENNIUM       ( 946684800l )
#define DAYS_SINCE_EPOCH_ON_MILLENNIUM          ( 10957l )
#define DIV_HZ_TO_MHZ   ( 1000ul * 1000ul )


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_UNITS_H
