#ifndef LEPTO_PRINT_H
#define LEPTO_PRINT_H
/**---------------------------------------------------------------------------
 *
 * @file    print.h
 * @brief   Helper functions to print stuff
 *
 * @date      20150711
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>


/*--- Declarations ---------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_HEX_DUMP_RANGE_COLOR )

struct SHexDumpRange
{
   int start;
   int size;
   const char* colorCode;
};

#endif

void hexDump(const void *buf, int size, bool printHeader=true
   #if IS_ENABLED( CONFIG_LEPTO_HEX_DUMP_RANGE_COLOR )
             , SHexDumpRange* range = nullptr
   #endif
);


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_PRINT_H
