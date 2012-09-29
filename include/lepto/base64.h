#ifndef LEPTO_BASE64_H
#define LEPTO_BASE64_H
/**---------------------------------------------------------------------------
 *
 * @file       base64.h
 * @brief      Base64 encoder/decoder
 *
 * @date       20170814
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>

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
      static const char *alphabet;
   public:
      static int encode(const uint8_t *src, size_t srcSize, char *dest, size_t destSize);
      static int encode(const CByteArray &src, CString &dest);
      static int decode(const char *src, size_t srcSize, uint8_t *dest, size_t destSize);
      static int decode(const CString &src, CByteArray &dest);
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_BASE64_H
