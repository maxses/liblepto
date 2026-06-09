/**---------------------------------------------------------------------------
 *
 * @file    print.h
 * @brief   Helper functions to print stuff
 *
 * @date      20150711
 * @author    Maximilian Seesslen <src@seesslen.net>
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
#include <lepto/ansi.h>


/*--- Implementation -------------------------------------------------------*/


void hexDump(const void *_buf, int size, bool printHeader /*=true*/
         #if IS_ENABLED( CONFIG_LEPTO_HEX_DUMP_RANGE_COLOR )
                     , SHexDumpRange* range
         #endif
             
)
{
   uint8_t *buf=(uint8_t *)_buf;
   if( printHeader )
   {
      // Generate output for
      // "    | -0 -1 -2 -3 -4 -5 -6 -7 -8 -9 -A -B -C -D -E -F\n"
      // "----|--------------------------------------------------\n"
      printf("    | ");
      for(int i1=0; i1<0x10; i1++)
      {
         printf("-%X ", i1);
      }
      printf("\n");
      printf( "----|-" );
      for(int i1=0; i1<0x10; i1++)
      {
         printf("---");
      }
      printf("|\n");
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
      for(int i2=0; (i2<0x10); i2++)
      {
         if( off < size )
         {
            #if IS_ENABLED( CONFIG_LEPTO_HEX_DUMP_RANGE_COLOR )
            if(range)
            {
               for(int i3=0; range[i3].start>=0; i3++)
               {
                  if( ( off >= range[i3].start ) && ( off < range[i3].start + range[i3].size ) )
                  {
                     fputs( range[i3].colorCode, stdout );
                     break;
                  }
               }
            }
            #endif
            
            printf("%02X ", buf[off]);
            
            #if IS_ENABLED( CONFIG_LEPTO_HEX_DUMP_RANGE_COLOR )
               printf( ANSI_NORMAL );
            #endif
            
            off++;
            line++;
         }
         else
         {
            printf("   ");
         }
      }
      
      off-=line;
      printf("| ");

      for(int i2=0; i2 < 0x10; i2++)
      {
         if(  off < size )
         {
            if( ( buf[off] >= ' ' ) && ( buf[off] < 0x7F ) )
            {
               printf("%c", buf[off]);
            }
            else
            {
               printf(".");
            }
            off++;
         }
         else
         {
            printf(" ");
         }
      }

      printf(" |\n");
   }

   return;
}


/*--- Fin ------------------------------------------------------------------*/
