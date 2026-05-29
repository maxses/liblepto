#ifndef LEPTO_BASE64_H
#define LEPTO_BASE64_H
/**---------------------------------------------------------------------------
 *
 * @file       base64.h
 * @brief      Base64 encoder/decoder
 *
 * @date       20170814
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <lepto/lepto.h>

#if defined( STM32 )
   #include <lepto/string.hpp>
   #include <lepto/mockQt.hpp>
#else
   // #include <QString>
   #include <lepto/string.hpp>
#endif


/*--- Declarations ---------------------------------------------------------*/


class CBase64
{
   private:
      #if IS_ENABLED( CONFIG_LEPTO_BASE64_STATIC_ALPHABET )
      static const char *m_alphabet;
      #endif // ? CONFIG_LEPTO_BASE64_STATIC_ALPHABET
   public:
      static char alphabet(char index);
      static int encode(const uint8_t *src, size_t srcSize, char *dest, size_t destSize);
      static int encode(const CByteArray &src, CString &dest);
      static int decode(const char *src, size_t srcSize, uint8_t *dest, size_t destSize);
      static int decode(const CString &src, CByteArray &dest);
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_BASE64_H
