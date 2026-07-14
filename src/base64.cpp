/**---------------------------------------------------------------------------
 *
 * @file       base64.cpp
 * @brief      Base64 encoder/decoder
 *
 * @date       20170814
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>           // uint8_t
//#include <stdlib.h>           // size_t
#include <lepto/base64.h>


/*--- Declarations ---------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_BASE64_STATIC_ALPHABET )
/* static */
const char *CBase64::m_alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/=";
#endif // ? CONFIG_LEPTO_BASE64_STATIC_ALPHABET


#if IS_ENABLED( CONFIG_LEPTO_BASE64_STATIC_ALPHABET )

char CBase64::alphabet(char index)
{
   return( m_alphabet[ index ] );
}

#else

char CBase64::alphabet(char index)
{
   if( (int8_t)index >= 0 )
   {
      // 0 ... 25
      if( index <=25 )
         return 'A' + ( index );

      // 26 ... 51
      if( index <= 51 )
         return 'a' + ( index - 26 );

      // 52 ... 61
      if( index <= 61 )
         return '0' + ( index - 52 );
   }

   // default
   return "+/="[ index - 62 ];

   /*
   switch( index )
   {
      case 0 ... 25:
         return 'A' + ( index );
         break;
      case 26 ... 51:
         return 'a' + ( index - 26 );
         break;
      case 52 ... 61:
         return '0' + ( index - 52 );
         break;
      default:
         return "+/="[ index - 62 ];
         break;
   }
   */
}

#endif // ? CONFIG_LEPTO_BASE64_STATIC_ALPHABET ELSE

uint32_t CBase64::triPortion( const uint8_t *src, lsize_t srcSize )
{
   uint32_t value=0;
   
   for(lsize_t i1=0; i1<3; i1++)
   {
      value=(value<<8) |
              ( i1 < srcSize
                   ?(uint8_t)src[ i1 ] : 0 );
   }
   
   return( value );
}


char CBase64::alphabetIndex(int value, int pos)
{
   return( (value >> ((3-pos)*6)) & 0x3f );
}


int CBase64::encode(const uint8_t *src, lsize_t srcSize, char *dest, lsize_t destSize) /**/
{
   lsize_t destPos=0;
   lsize_t srcPos=0;
   uint32_t value;

   while(srcPos<(int)srcSize)
   {
      value = triPortion( &src[ srcPos ], srcSize - srcPos );

      for(uint32_t i1=0; i1<4; i1++)
      {
         // The filling byte is valuated for the four bytes by this behaviour:
         // <0 <0 <1 <2; therefore the srcPos is incremented starting with the
         // second output char.
         if(srcPos<(int)srcSize)
         {
            dest[destPos++]=alphabet( alphabetIndex(value, i1) );
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
      value = triPortion( (uint8_t*)&src.data()[ srcPos ], srcSize - srcPos );

      for(uint32_t i1=0; i1<4; i1++)
      {
         // The filling byte is valuated for the four bytes by this behaviour:
         // <0 <0 <1 <2; therefore the srcPos is incremented starting with the
         // second output char.
         if(srcPos<(int)srcSize)
         {
            dest+=alphabet( alphabetIndex(value, i1) );
         }
         else
            dest+='=';
         if(i1)
            srcPos++;
      }
   }
   return( dest.length() );
}


int CBase64::decode(const char *src, lsize_t srcSize, uint8_t *dest, lsize_t destSize)
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
            // Hasndle "+/="
            case '+':
               // [fall-through]
            case '/':
               rawValue=(src[srcPos]-'+')+(26*2)+10;
               break;
            case '=':
               rawValue=0;
               break;
            default:
               return(-1);
               break;
         }
         value= (value<<6) | ( rawValue & 0x3f );
         if( src[srcPos] == '=' )
            reduced++;
         srcPos++;
      }

      for(int i2=0; ( i2 < 3 ) && (destPos+i2<(int)destSize); i2++)
      {
         dest[destPos+i2]= ( value >> ( (2-i2) * 8 ) )&0xff;
      }
      destPos+=3-reduced;

      if( (destPos>(int)destSize) && ( srcPos<(int)srcSize ) )
      {
         destPos=-1;
         break;
      }
   }

   return(destPos);
}


lsize_t CBase64::decode(const CString &src, CByteArray &dest)
{
   int srcPos=0, i1, reduced;
   uint32_t value;
   uint32_t rawValue;
   lsize_t srcSize=src.length();

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
         dest+= ( char )( ( value >> ( (2-i2) * 8 ) ) & 0xff );
   }
   return(dest.length());
}


/*--- Fin ------------------------------------------------------------------*/
