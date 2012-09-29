#ifndef LEPTO_CRC32_H
#define LEPTO_CRC32_H
/**---------------------------------------------------------------------------
 *
 * @file       crc32.h
 * @brief      Functions for CRC32 calculation in software
 *
 * By default an static table is used for calcilations.
 * The default CRC32 setting is 'CRC-32/ISO-HDLC'.
 * The functions are C functions so other libraries can remap them to functions
 * with hardware support like from libbiwak.
 *
 * Usecases:
 *    - Data verification
 *    - Validation of EEPROM content
 *
 * Configuration defines:
 *    - CONFIG_LEPTO_CRC32_CONFIGURABLE
 *       Use function pointers for crc32 functions. THe function 'configCrc32()'
 *       can be used to change the used CRC32 settings; either default or MPEG.
 *       Some STM32 only support MPEG in hardware.
 *
 * @date       20211127
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <cstdint>
#include <lepto/lepto.h>

// For regular linux builds 'LEPTO_CONFIGURABLE_CRC32' should be enabled by
// default. The 'hwdata' tool is using it.
#if ! defined (STM32)
   #if ! defined CONFIG_LEPTO_CRC32_CONFIGURABLE
      #define CONFIG_LEPTO_CRC32_CONFIGURABLE            1
   #endif
#endif


/*--- Declaration ----------------------------------------------------------*/


typedef uint32_t crc32_t;

// Make them "C" to avoid name mangeling. Arena may map the symbols to biwak
// functions.
extern "C"
{
   #if IS_ENABLED( CONFIG_LEPTO_CRC32_CONFIGURABLE )
   extern crc32_t (*crc32Init)( );
   extern crc32_t (*crc32Update)( crc32_t crc, const void *buf, uint32_t len );
   extern crc32_t (*crc32Finalize)( crc32_t crc );
   extern crc32_t (*calcCrc32)( const void *buf, uint32_t len );

   enum ECrc32Mode {
      Default,
      Mpeg,
   };

   void configCrc32( ECrc32Mode mode);
   #else // ? CONFIG_LEPTO_CRC32_CONFIGURABLE
   crc32_t crc32Init( );
   crc32_t crc32Update( crc32_t crc, const void *buf, uint32_t len );
   crc32_t crc32Finalize( crc32_t crc );
   crc32_t calcCrc32( const void *buf, uint32_t len );
   #endif // ! ? CONFIG_LEPTO_CRC32_CONFIGURABLE

   crc32_t crc32DefaultInit( );
   crc32_t crc32DefaultUpdate( crc32_t crc, const void *buf, uint32_t len );
   crc32_t crc32DefaultFinalize( crc32_t crc );
   crc32_t calcCrc32Default( const void *buf, uint32_t len );

   crc32_t crc32MpegInit( );
   crc32_t crc32MpegUpdate( crc32_t crc, const void *buf, uint32_t len );
   crc32_t crc32MpegFinalize( crc32_t crc );
   crc32_t calcCrc32Mpeg( const void *buf, uint32_t len );
}


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_CRC32_H
