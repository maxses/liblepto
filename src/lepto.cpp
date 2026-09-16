/**---------------------------------------------------------------------------
 *
 * @file       lepto.cpp
 * @brief      General defines
 *
 * No functions left over.
 *
 * @date       20240024
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>
#include <lepto/log.h>


/*--- Implementation -------------------------------------------------------*/


void leptoInit()
{
   leptoInitLog();
}


void leptoEventLoop()
{
   logEventLoop();
}


uint32_t leptoConfigChecksum()
{
   return( LEPTO_CONFIG_CHECKSUM );
}

uint32_t leptoCodeVersion()
{
   return( LEPTO_CODE_SHA );
}



/*--- Fin ------------------------------------------------------------------*/
