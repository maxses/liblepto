#ifndef LEPTO_STRING_HPP
#define LEPTO_STRING_HPP
/**---------------------------------------------------------------------------
 *
 * @file    string.hpp
 * @brief   Class for text strings
 *
 * The implementation is a little bit outdated. A big cleanup would be
 * appropriate.
 * Some functions are just (intended) dummy to provide compability with Qt.
 *
 * This class is used in 'libfosh'.
 *
 * @date   20150622
 * @author Maximilian Seesslen <mes@seesslen.net>
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


/*--- Defines --------------------------------------------------------------*/


// Too small values actually waste RAM due to fragmentation. even 10 is bad
#define MEMORY_GAP   20
#define  CONFIG_LEPTO_STRING_FREE_ON_CLEAR         0  // costs 8 bytes but may
                                                      // lead to fragmentation
#define CONFIG_LEPTO_STRING_CACHED_LENGTH          1  // brings bytes


/*--- Declarations ---------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_CCHAR )

class CChar
{
   private:
      char m_char;

   public:
      CChar(char _char)
      {
         m_char=_char;
      }

      int unicode() const
      {
         return(m_char);
      }
      unsigned char cell() const
      {
         return( (unsigned char)m_char );
      }
      bool isLetter() const
      {
         return(
                  ( (m_char>='a') && (m_char<='z') )
               || ( (m_char>='A') && (m_char<='Z') )
         );
      }
      int digitValue() const
      {
         if( (m_char>='0') && (m_char<='9') )
            return(m_char - '0');
         return(-1);
      }
      bool operator ==(const char &c) const
      {
         return( m_char == c );
      }
};

#endif // ? CONFIG_LEPTO_CCHAR


template <typename T>
class CBaseString
{
   friend class CString;

   protected:
      T *m_buf;

   private:
      //static const T * const staticZeroString;
      #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
      int m_length;
      #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
      int m_maxSize;

   public:
      constexpr CBaseString();
      ~CBaseString();

      CBaseString(const T *buf);
      CBaseString(const CBaseString& r)
      {
         *this = r;
      }

      CBaseString& operator= (const CBaseString& r)
      {
         checkSpace(r.length(), false);
         memcpy(m_buf, r.m_buf, r.length()+1);
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
            m_length=strlen(m_buf);
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         return( *this );
      }
      void clear();
      int length() const
      {
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
            return( m_length );
         #else
            return( strlen(m_buf) );
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
      }
      int maxLength() const
      {
         return(m_maxSize-1);
      }
      int maxSize() const
      {
         return(m_maxSize);
      }
      /*
      int setLength(int length)
      {
         checkSpace(length);
         return(m_length=length);
      }
      */

      /**
       * @brief Allocate new memory. Old data is not handles.
       * @param size    New total maximum length.
       */
      void allocate(int size);

      /**
       * @brief   Check for availability for (additional) space
       *
       *          Allocate more memory when space is not long enough.
       * @param   length      Total length of the new string
       * @param   doPreserve  If set the the old data will be copied when
       *                      reallocating space.
       */
      void checkSpace(int length, bool doPreserve = true);

      /**
       * @brief   Cut the sttring at a position
       * @param   pos: New length of the string
       */
      void cut(int pos)
      {
         if(pos<=m_maxSize)
         {
            m_buf[pos]=0;
            #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
            m_length=pos;
            #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         }
      }

      // Well, Qt...
      #if 0
      CChar operator[](int pos) const
      {
         //return( m_buf[pos] );
         return( CChar(m_buf[pos]) );
      };
      #else
      char operator[](int pos) const
      {
         return( m_buf[pos] );
      };
      #endif

      T *data() const
      {
         return(m_buf);
      }
      #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
      T pop_bottom()
      {
         T t=(T)0;
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
         if(m_length)
         {
            t=m_buf[--m_length];
            m_buf[m_length]=0;
         }
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         return(t);
      }
      #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH

      CBaseString<T> &add(const T *_pChar, int len);

      /// \brief  Little hack; returns reference to itself
      ///
      ///         This method is needed to get the method food.toUtf8().data()
      ///          to work.
      CBaseString<T> &toUtf8()
      {
         return(*this);
      }

      CBaseString<T> &operator+=(const T _char);
      CBaseString<T> &operator+=(const T *_pChar);
      CBaseString<T> &operator+=(const CBaseString<T>& r)
      {
         checkSpace( length() +  r.length() + 1);
         memcpy(&m_buf[ length() ], r.data(), r.length()+1);
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
            m_length+=r.length();
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         return *this;
      }

      #if IS_ENABLED( CONFIG_LEPTO_CCHAR )
         CBaseString<T> &operator+=(const CChar _CChar);
      #endif

      CBaseString<T> &operator<<(const T _char);
      CBaseString<T> &operator<<(const uint32_t _int);
      CBaseString<T> &operator<<(const int32_t _int);

      void setData(const T *_data, int len);
      CBaseString<T> &append(const T *_pChar, int len);
      CBaseString<T> &append(const CBaseString<T> &_data);
      CBaseString<T> &insert(int insertPos, T _char);
      CBaseString<T> &remove(int pos, int number);

};


class CString: public CBaseString<char>
{
   public:
      constexpr CString(){};
      ~CString();
      CString(const char *input)
         :CBaseString(input)
      {
      };

      CString(const CString& input):CBaseString(input)
      {
      };

      #if 0
      int printf(const char format[], ...)
      {
         va_list va;
         int newSize;
         int oldLength=0; // length();

         va_start(va, format);
         // the +32 is not the gap, its the assumed arguments overhead
         checkSpace( length() + strlen(format) + 32 );
         newSize=vsnprintf(&data()[oldLength], maxSize() - oldLength, format, va);
         if(newSize>maxLength()+oldLength)
         {
            while(1)
            {}
         }
         m_length+=newSize;

         va_end(va);

         return(0);
      }
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_CCHAR )
         const CChar at(int pos) const
         {
            return( CChar(m_buf[pos]) );
         };
      #endif // ? CONFIG_LEPTO_CCHAR

      char cell(int pos) const
      {
         return( m_buf[pos] );
      };

      bool operator== (const char* str) const
      {
         int pos=0;

         while(str[pos])
         {
            if( str[pos] != m_buf[pos])
            {
               return(false);
            }
            pos++;
         }
         // One string longer than the other?
         if( str[pos] != m_buf[pos])
         {
            return(false);
         }

         return(true);
      }

      CString& toLatin1()
      {
         return(*this);
      }

      CString& operator+(const CString& r2)
      {
         (*this)+=r2;
         return(*this);
      }
};


class CByteArray: public CBaseString<char>
{
   public:
      CByteArray();
      ~CByteArray();
      CByteArray(const char *buf)
         :CBaseString<char>(buf)
      {
      }

      char at(int pos) const
      {
         return( m_buf[pos] );
      };
};


template <typename T>
constexpr CBaseString<T>::CBaseString()
   //:m_buf( ( T *) staticZeroString )
   :m_buf( nullptr )
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
   ,m_length(0)
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
   ,m_maxSize(0)
{
}


template <typename T>
CBaseString<T>::CBaseString(const T *buf)
{
   int i1=strlen(buf)+1;
   allocate(i1);
   memcpy(m_buf, buf, i1);
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
   m_length=i1-1;
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
}


template <typename T>
CBaseString<T>::~CBaseString()
{
   clear();
   if(m_buf)
   {
      free(m_buf);
   }
   m_buf=nullptr;
}


template <typename T>
void CBaseString<T>::clear()
{
   if( m_buf )
   {
      #if IS_ENABLED( CONFIG_LEPTO_STRING_FREE_ON_CLEAR )
      if( m_buf != nullptr )
      {
         free(m_buf);
         m_buf = nullptr;
         m_maxSize = 0;
      }
      else
      #endif
      {
         m_buf[0]=0;
      }
   }
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
   m_length=0;
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
}


template <typename T>
void CBaseString<T>::allocate(int size)
{
   // Must not free m_buf here. Other methods may want to copy from old area
   m_maxSize=size;
   m_buf=(T *)malloc(m_maxSize);
   if(!m_buf)
   {
      abort();
   }
   m_buf[0]=0;
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
   m_length=0;
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
   return;
}


template <typename T>
void CBaseString<T>::checkSpace(int newLength, bool doPreserve /*=true*/ )
{
   T *oldBuf=m_buf;
   int oldLength=length();

   // We need size+1 to keep a zero
   if(newLength+1>m_maxSize)
   {
      // Only extend with a gap when there is a byte by byte increasement.
      if( newLength+1 == m_maxSize + 1  )
      {
         newLength+=MEMORY_GAP;
      }
      allocate( newLength + 1 );
      if( doPreserve )
      {
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
         m_length=oldLength;
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         if( oldBuf )
         {
            memcpy(m_buf, oldBuf, oldLength+1);
         }
         else
         {
            // memset(m_buf, 'X', m_length);
            m_buf[0]=0;
         }
      }
      else
      {
         #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
         m_length=0;
         #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
         m_buf[0]=0;
      }

      if( oldBuf )
      {
         free(oldBuf);
      }
   }

   return;
}


template <typename T>
CBaseString<T> &CBaseString<T>::operator +=(const T _char)
{
   int end=length();
   checkSpace(end+1);
   m_buf[end]=_char;
   m_buf[++end]=0;
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
   m_length=end;
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH

   return(*this);
}


template <typename T>
CBaseString<T> &CBaseString<T>::operator <<(const T _char)
{
   return( ( (*this) += _char ) );
}


template <typename T>
CBaseString<T> &CBaseString<T>::operator +=(const T *_pChar)
{
   return( append(_pChar, strlen(_pChar)) );
}


template <typename T>
CBaseString<T> &CBaseString<T>::append(const T *_pChar, int len)
{
   int oldLength=length();
   checkSpace(oldLength+len);
   memcpy(&m_buf[oldLength], _pChar,len);

   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
      m_length+=len;
      m_buf[m_length]=0;
   #else
      m_buf[oldLength+len]=0;
   #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH

   return(*this);
}


template <typename T>
CBaseString<T> &CBaseString<T>::append(const CBaseString<T> &_data)
{
   return( append(_data.data(), _data.length()) );
}

#if IS_ENABLED( CONFIG_LEPTO_CCHAR )

template <typename T>
CBaseString<T> &CBaseString<T>::operator+=(const CChar _CChar)
{
   checkSpace(m_length+1);
   m_buf[m_length++]=_CChar.unicode();
   m_buf[m_length]=0;

   return(*this);
}

#endif // ? CONFIG_LEPTO_CCHAR

template <typename T>
void CBaseString<T>::setData(const T *_data, int len)
{
   checkSpace(len);
   memcpy(m_buf, _data, len);
   #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
      m_length=len;
   #endif // ? CONFIG_LEPTO_STRING_CACHED_LENGTH
   m_buf[ len ]=0;
}


template <typename T>
CBaseString<T>& CBaseString<T>::remove(int pos, int number)
{
   if(pos<0)
   {
      pos=length()+pos;
   }

   // "abcdefgh"; length=8; pos=6;
   number=MIN(number, length()-pos);
   if( (pos>=0) && pos<length())
   {
      for(int i1=pos; i1<length(); i1++ )
      {
         m_buf[i1]=m_buf[i1+number];
      }
      #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
         m_length=strlen( m_buf );
      #endif // ? CONFIG_LEPTO_STRING_CACHED_LENGTH
   }
   return(*this);
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_STRING_HPP
