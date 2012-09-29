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


/*--- Defines --------------------------------------------------------------*/


#include <lepto/print.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>                    // malloc
#include <lepto/lepto.h>
#include <stdint.h>                    // uint8_t


/*--- Implementation -------------------------------------------------------*/


void hexDump(const void *_buf, int size, bool printHeader /*=true*/)
{
   uint8_t *buf=(uint8_t *)_buf;
   if( printHeader )
   {
      printf("    | -0 -1 -2 -3 -4 -5 -6 -7 -8 -9 -A -B -C -D -E -F\n");
      printf("----|------------------------------------------------\n");
   }
   int rows=size/0x10;
   int off=0;
   if( size % 0x10 )
   {
      rows++;
   }

   for(int i1=0; i1<rows; i1++)
   {
      int line=0;
      printf(" %X- | ", i1);
      for(int i2=0; (i2<0x10) && ( off < size); i2++)
      {
         printf("%02X ", buf[off]);
         off++;
         line++;
      }
      off-=line;
      printf(" | ");

      for(int i2=0; (i2<0x10) && ( off < size); i2++)
      {
         if( ( buf[off] >= 0x20 ) && ( buf[off] < 0xFF ) )
         {
            printf("%c", buf[off]);
         }
         else
         {
            printf(".");
         }
         off++;
      }

      printf("\n");
   }

   return;
}


/*--- Fin ------------------------------------------------------------------*/
