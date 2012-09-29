/**---------------------------------------------------------------------------
 *
 * @file       base64.cpp
 * @brief      Base64 encoder/decoder
 *
 * @date       20170814
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>           // uint8_t
//#include <stdlib.h>           // size_t
#include <lepto/base64.h>


/*--- Declarations ---------------------------------------------------------*/


/* static */
const char *CBase64::alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/=";


int CBase64::encode(const uint8_t *src, size_t srcSize, char *dest, size_t destSize) /**/
{
   int destPos=0, srcPos=0;
   uint32_t value;

   while(srcPos<(int)srcSize)
   {
      value = 0;
      for(uint32_t i1=0; i1<3; i1++)
      {
         value=(value<<8) | (uint8_t)src[srcPos+i1];
      }

      for(uint32_t i1=0; i1<4; i1++)
      {
         // The filling byte is valuated for the four bytes by this behaviour:
         // <0 <0 <1 <2; therefore the srcPos is incremented starting with the
         // second output char.
         if(srcPos<(int)srcSize)
         {
            dest[destPos++]=alphabet[ (value >> ((3-i1)*6)) & 0x3f ];
         }
         else
            dest[destPos++]='=';
         if(i1)
            srcPos++;
         if(destPos>=(int)destSize)
         {
            destPos=-3;
            srcPos=srcSize;
            break;
         }
      }
   }
   if(destPos>=0)
      dest[destPos]=0;
   return(destPos);
}


int CBase64::encode(const CByteArray &src, CString &dest) /**/
{
   int srcPos=0;
   int srcSize=src.length();
   uint32_t value;

   while(srcPos<(int)srcSize)
   {
      value = 0;
      for(int i1=0; i1<3; i1++)
      {
         value=(value<<8) |
               ( ( srcPos+i1 ) < srcSize
                  ?(uint8_t)src.at(srcPos+i1):0);
      }

      for(uint32_t i1=0; i1<4; i1++)
      {
         // The filling byte is valuated for the four bytes by this behaviour:
         // <0 <0 <1 <2; therefore the srcPos is incremented starting with the
         // second output char.
         if(srcPos<(int)srcSize)
         {
            dest+=alphabet[ (value >> ((3-i1)*6)) & 0x3f ];
         }
         else
            dest+='=';
         if(i1)
            srcPos++;
      }
   }
   return( dest.length() );
}


int CBase64::decode(const char *src, size_t srcSize, uint8_t *dest, size_t destSize)
{
   int destPos=0, srcPos=0, i1, reduced;
   uint32_t value;
   uint32_t rawValue;

   while(srcPos<(int)srcSize)
   {
      value=0;
      reduced=0;
      for(i1=0; i1<4; i1++)
      {
         switch(src[srcPos])
         {
            case 'A'...'Z':
               rawValue=(src[srcPos]-'A');
               break;
            case 'a'...'z':
               rawValue=(src[srcPos]-'a')+26;
               break;
            case '0'...'9':
               rawValue=(src[srcPos]-'0')+(26*2);
               break;
            case '+':
               rawValue=(src[srcPos]-'+')+(26*2)+10;
               break;
            case '/':
               rawValue=(src[srcPos]-'/')+(26*2)+10+1;
               break;
            default:
               rawValue=0;
               break;
         }
         value= (value<<6) | ( rawValue & 0x3f );
         if( src[srcPos] == '=' )
            reduced++;
         srcPos++;
      }

      for(int i2=0; (i2<3) && (destPos+i2<(int)destSize); i2++)
      {
         dest[destPos+i2]= ( value >> ( (2-i2) * 8 ) )&0xff;
      }
      destPos+=3-reduced;

      if(destPos>(int)destSize)
      {
         destPos=-1;
         break;
      }
   }
   return(destPos);
}


int CBase64::decode(const CString &src, CByteArray &dest)
{
   int srcPos=0, i1, reduced;
   uint32_t value;
   uint32_t rawValue;
   int srcSize=src.length();

   while(srcPos<(int)srcSize)
   {
      value=0;
      reduced=0;
      for(i1=0; i1<4; i1++)
      {
         unsigned char cell=src.cell( srcPos );
         switch( cell )
         {
            case 'A'...'Z':
               rawValue=(cell-'A');
               break;
            case 'a'...'z':
               rawValue=(cell-'a')+26;
               break;
            case '0'...'9':
               rawValue=(cell-'0')+(26*2);
               break;
            case '+':
               rawValue=(cell-'+')+(26*2)+10;
               break;
            case '/':
               rawValue=(cell-'/')+(26*2)+10+1;
               break;
            default:
               rawValue=0;
               break;
         }
         value= (value<<6) | ( rawValue & 0x3f );
         if( src[srcPos] == '=' )
            reduced++;
         srcPos++;
      }

      /*
      for(int i2=0; (i2<3) && (destPos+i2<(int)destSize); i2++)
      {
         dest[destPos+i2]= ( value >> ( (2-i2) * 8 ) )&0xff;
      }
      destPos+=3-reduced;
      */

      for(int i2=0; i2 < (3-reduced) ; i2++)
         dest+= ( value >> ( (2-i2) * 8 ) )&0xff;
   }
   return(dest.length());
}


/*--- Fin ------------------------------------------------------------------*/
