#ifndef LEPTO_STRING_HPP
#define LEPTO_STRING_HPP
/**---------------------------------------------------------------------------
 *
 * @file    string.hpp
 * @brief   Class for text strings
 *
 * The class inherits CList.
 * When data is allocated, there is always included and trailing zero. When
 * string is empty, data might be a null pointer.
 *
 * Some functions are just (intended) dummy to provide compability with Qt.
 *
 * This class is used in 'libfosh' for example.
 *
 * @date   20150622
 * @author Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdlib.h>           // malloc, free
#include <string.h>           // memcpy
#include <stdint.h>           // uint32_t
#include <stdarg.h>
#include <stdio.h>            // vsnprintf
#include <lepto/lepto.h>      // IS_ENABLED
#include <lepto/list.hpp>       // CONFIG_LEPTO_LIST_AS_STRING


/*--- Defines --------------------------------------------------------------*/


// Too small values actually waste RAM due to fragmentation.
// 16 gaves best result on 'miniminutnik'. ( Heap usage is not linear)
#if ! defined CONFIG_LEPTO_STRING_MEMORY_HIKE
   #define CONFIG_LEPTO_STRING_MEMORY_HIKE   16
#endif
#define  CONFIG_LEPTO_STRING_FREE_ON_CLEAR         1  // costs 8 bytes but may
                                                      // lead to fragmentation
#define CONFIG_LEPTO_STRING_CACHED_LENGTH          1  // brings bytes


/*--- Declarations ---------------------------------------------------------*/


class CString: private CList<char>
{
   private:
      // Avoid magic numbers
      static constexpr ringIndex_t TRAILING_ZERO=1;

   public:
      /**
       * @brief Get the real end index of string
       *
       * Pint either to the trailing zero or to 0 if list is empty.
       * @return index
       */
      ringIndex_t realEnd()
      {
          return( m_backPos - ( m_backPos ? TRAILING_ZERO : 0) );
      }

   public:
      constexpr int length() const
      {
         int c=count();

         // Ignore the trailing zero
         if( c )
         {
             c--;
         }

         return( c );
      }
      char* data() const // Not a "const char*"; e.g. libfosh manipulates the string
      {
         // lAssert( m_backPos >= m_frontPos );
         // TBD: check overflow
         lHostAssert( m_backPos >= m_frontPos );
         lHostAssert( m_frontPos == 0 );

         // Checks costs 24 bytes. Returning 'nullptr' is valid.
         return( m_buffers );
      }
      CString( )
      {};
      CString( int length ): CList( length + ( length ? ( TRAILING_ZERO  ): 0 ) )
      {};
      CString( const char* str ): CList( strlen( str ) + TRAILING_ZERO )
      {
         tryAppend(str);
      };
      
      #if defined STM32
      
      CString(const CString&) = delete;

      #else
      
      CString( const CString& str ): CList( str.count() /*str.getMaxEntries()*/ )
      {
         memcpy( getBuffers(), str.getBuffers(), str.count() );
         m_backPos=str.m_backPos;
      };

      #endif
      
      CString& operator +=(char c)
      {
         replaceBack( c );
         push_back( 0 );
         return( *this );
      };

      CString& operator +=(const char* str)
      {
         tryAppend( str );
         return( *this );
      }
      CString& operator +=( const CString& str)
      {
          tryAppend( str.data() );
         return( *this );
      }
      #if ! defined STM32
      CString operator +( const CString& strB ) const
      {
         // Let it work with non-resizable configuration
         CString str( length() + strB.length() + TRAILING_ZERO );
         str=*this;
         str+=strB;
         return( str );
      }
      #endif
      char at(int pos) const
      {
         return( m_buffers[ pos ] );
      };
      char operator[](int pos) const
      {
         return( at( pos ) );
      };
      bool operator==(const char* str) const
      {
         if(!m_buffers)
         {
            return(false);
         }
         #if 0
            return( strcmp( getBuffers(), str ) == 0 );
         #else
            int pos=-1;

            do
            {
               pos++;
               if( str[pos] != m_buffers[pos] )
               {
                  return(false);
               }
            }while(str[pos]);

            return( m_buffers[pos] == 0 );
         #endif
      }
      void remove(int pos, int size)
      {
         if(pos<0)
         {
            pos=length()+pos;
            /*
            m_backPos-=size;
            if( m_backPos < 0 )
            {
               m_backPos=0;
            }
            memset( &(getBuffers()[m_backPos]), 0, getMaxEntries() - m_backPos);
            */
         }

         if( pos + size >= length() )
         {
            getBuffers()[pos]=0;
            m_backPos=pos+1;
         }
         else
         {
            memcpy( &(getBuffers()[pos]), &(getBuffers()[pos+size]),
                     length()-pos-size );

            // Caution: probably no sign available in position type
            if( m_backPos >= (ringIndex_t)size )
            {
               m_backPos-=size;
            }
            else
            {
               m_backPos = TRAILING_ZERO;
            }
            //memset( &(getBuffers()[m_backPos]), 0, getMaxEntries() - m_backPos);
         }
         m_buffers[ m_backPos - TRAILING_ZERO ]=0;
      }
      void operator =(const char* str)
      {
         m_frontPos = m_backPos = 0;
         tryAppend(str);
         return;
      }
      bool tryAppend( const char* str )
      {
          int size=strlen(str) + TRAILING_ZERO;

          if( ! checkSpace( length() + size ) )
          {
             return( false );
          }
          memcpy(&m_buffers[ realEnd() ], str, size);
          m_frontPos = 0;
          if( m_backPos )
          {
              m_backPos += size - 1;
          }
          else
          {
            m_backPos = size;
          }
          return( true );
      }
      #if defined(STM32)
         CString& operator=(CString const&) = delete;
      #else
         CString& operator=(CString const &str )
         {
             tryAppend( str.data() );
             return( *this );
         }
      #endif
         
      void clear()
      {
         CList::clear();
         if( m_maxEntries )
         {
            m_buffers[0]=0;
         }
      }
      
      /**
       * @brief Get the back position for debugging
       * @return backPos
       */
      int backPos()
      {
          return( m_backPos );
      }
};

class CByteArray: public CString
{
   public:
      CByteArray( const char* str ): CString(str)
      {
      }

      CByteArray( )
      {
      }

      CByteArray( int length ): CString(length)
      {
      }

   /*
      int length() const
      {
         return( count() );
      }
   */
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_STRING_HPP
