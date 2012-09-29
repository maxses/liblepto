#ifndef LEPTO_LEPTO_HPP
#define LEPTO_LEPTO_HPP
/**---------------------------------------------------------------------------
 *
 * @file       lepto.h
 * @brief      General defines
 *
 * Some defines.
 *
 * @date       20240024
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/configArgs.h>          // let IS_ENABLED work for compability


/*--- Defines --------------------------------------------------------------*/


#if ! defined(LEPTO_CONFIGURED)
   #error   LEPTO_CONFIGURED not defined. The configuration header was \
            probably not involved. If the file is present make sure the macro \
            'LEPTO_CONFIGURED' is defined in the configuration header.
#endif

#if ! defined (MAX)
   #define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if ! defined (MIN)
   #define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

// Because Qt has QT_STRINGIFY
#define L_STRINGIFY2(exp) "" #exp ""
#define L_STRINGIFY(exp) L_STRINGIFY2(exp)

#define arraySize(array) (int)(sizeof(array)/sizeof(array[0]) )

#if defined(_LP64)
   #include <stdint.h>
   #define address_t uint64_t
   #define FI32   "%d"
#else
   #define address_t uint32_t
   #define FI32   "%ld"
#endif

#define address32_t uint32_t

// This was for checking if some overloading worked correctly. May be removed soon.
#define LEPTO_CODE            0x1

// When CONFIG_LEPTO_LOG_USE_PRETTY_PRINT is not defined, enable it only when
// CONFIG_LEPTO_LOG_DOWNSIZE is not set.
// Make it a nice default configuration.
#if ! defined CONFIG_LEPTO_LOG_USE_PRETTY_PRINT
   #if ! IS_ENABLED( CONFIG_LEPTO_LOG_DOWNSIZE )
      #define CONFIG_LEPTO_LOG_USE_PRETTY_PRINT          1
   #endif
#endif // ? ! defined CONFIG_LEPTO_LOG_USE_PRETTY_PRINT


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LEPTO_HPP
