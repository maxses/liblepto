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
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/

/*--- Includes -------------------------------------------------------------*/

#include <assert.h>
#include <stdint.h>

#include <lepto/configArgs.h>

#if defined(LEPTO_GENERATED_CONFIG)
    #include "config_generated_lepto.h"
#else
    #include <lepto/config.h>
#endif

/*--- Defines --------------------------------------------------------------*/

#if !defined(LEPTO_CONFIGURED)
    #error   LEPTO_CONFIGURED not defined. The configuration header was \
             probably not involved. If the file is present make sure the macro \
             'LEPTO_CONFIGURED' is defined in the configuration header.
#endif

#if !defined(MAX)
    #define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#if !defined(MIN)
    #define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#define L_STRINGIFY2(exp) "" #exp ""
#define L_STRINGIFY(exp) L_STRINGIFY2(exp)
#define LEPTO_KEEP(sym) __asm__ __volatile__("" :: "m" (sym));
#define arraySize(array) (int)(sizeof(array) / sizeof(array[0]))

#if defined(_LP64)
    #define address_t uint64_t
    #define FI32      "%d"
#else
    #define address_t uint32_t
    #define FI32      "%ld"
#endif

#define address32_t uint32_t

#define LEPTO_CODE 0x1
#define lUNUSED(a) (void)a

typedef int lsize_t;

#if !defined(CONFIG_LEPTO_LOG_PRETTY_PRINT) && !IS_ENABLED(CONFIG_LEPTO_LOG_DOWNSIZE)
    #define CONFIG_LEPTO_LOG_PRETTY_PRINT 1
#endif

void leptoInit();
void leptoEventLoop();
uint32_t leptoConfigChecksum();
uint32_t leptoCodeVersion();

#define configHashCheck(module)                                        \
    do {                                                                \
        assert(module##_CONFIG_CHECKSUM == module##ConfigChecksum()); \
    } while (0)

#define codeVersionCheck(module)                                       \
    do {                                                                \
        assert(module##_CODE_SHA == module##CodeVersion());            \
    } while (0)

/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LEPTO_HPP
