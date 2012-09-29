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
#undef LEPTO_STRING_FREE_ON_CLEAR


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
      static const T * const staticZeroString;
      int m_length;
      int m_maxSize;

   public:
      constexpr CBaseString();
      ~CBaseString();

      CBaseString(const T *buf);
      CBaseString(const CBaseString&) = delete;

      void clear();
      int length() const
      {
         return(m_length);
      }
      int maxLength() const
      {
         return(m_maxSize-1);
      }
      int maxSize() const
      {
         return(m_maxSize);
      }
      int setLength(int length)
      {
         checkLength(length);
         return(m_length=length);
      }

      void allocate(int size);
      //void reserve(int size);
      void checkLength(int length);
      void cut(int i1)
      {
         if(i1<=m_maxSize)
            m_buf[i1]=0;
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
      T last()
      {
         T t=(T)0;
         if(m_length)
         {
            t=m_buf[--m_length];
            m_buf[m_length]=0;
         }
         return(t);
      }
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
};


class CString: public CBaseString<char>
{
   public:
      constexpr CString(){};
      ~CString();
      CString(const char *input)
         :CBaseString(input)
      {

      }
      #if 1
      int printf(const char format[], ...)
      {
         va_list va;
         int newSize;
         int oldLength=0; // length();

         va_start(va, format);
         // the +32 is not the gap, its the assumed arguments overhead
         checkLength( length() + strlen(format) + 32 );
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

//template <typename T>
//const T * const CBaseString<T>::staticZeroString=(T[]){(T)0};

template <typename T>
constexpr CBaseString<T>::CBaseString()
   //:m_buf( ( T *) staticZeroString )
   :m_buf( nullptr )
   ,m_length(0)
   ,m_maxSize(0)
{
   //allocate(0);
}


template <typename T>
CBaseString<T>::CBaseString(const T *buf)
{
   int i1=strlen(buf)+1;
   allocate(i1);
   memcpy(m_buf, buf, i1);
   m_length=i1-1;
}


template <typename T>
CBaseString<T>::~CBaseString()
{
   clear();
   free(m_buf);
   m_buf=nullptr;
}

template <typename T>
void CBaseString<T>::clear()
{
   if( m_buf )
   {
#if 0
      #if defined(LEPTO_STRING_FREE_ON_CLEAR)
      if( m_buf != staticZeroString )
      {
         free(m_buf);
         m_buf = (T *) staticZeroString ;
      }
      else
      #endif
#endif
      {
         m_buf[0]=0;
      }
   }
   m_length=0;
}


template <typename T>
void CBaseString<T>::allocate(int size)
{
   // Must not free m_buf here. Other methods may want to copy from old area
   m_maxSize=size+MEMORY_GAP;
   m_buf=(T *)malloc(m_maxSize);
   if(!m_buf)
   {
      abort();
   }
   m_buf[0]=0;
   m_length=0;
   return;
}

template <typename T>
void CBaseString<T>::checkLength(int length)
{
   T *oldBuf=m_buf;
   int oldLength=m_length;

   // We need size+1 to keep a zero
   if(length+1>m_maxSize)
   {
      allocate( length + 1 );
      m_length=oldLength;
      if( oldBuf )
      {
         memcpy(m_buf, oldBuf, m_length);
         free(oldBuf);
      }
      else
      {
         memset(m_buf, 'X', m_length);
      }
      m_buf[m_length]=0;
   }

   return;
}


template <typename T>
CBaseString<T> &CBaseString<T>::operator +=(const T _char)
{
   checkLength(m_length+1);
   m_buf[m_length++]=_char;
   m_buf[m_length]=0;

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
   checkLength(m_length+len);
   memcpy(&m_buf[m_length], _pChar,len);
   m_length+=len;
   m_buf[m_length]=0;

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
   checkLength(m_length+1);
   m_buf[m_length++]=_CChar.unicode();
   m_buf[m_length]=0;

   return(*this);
}

#endif // ? CONFIG_LEPTO_CCHAR

template <typename T>
void CBaseString<T>::setData(const T *_data, int len)
{
   checkLength(len);
   memcpy(m_buf, _data, len);
   m_length=len;
   m_buf[m_length+1]=0;

}


/*
template <typename T>
CBaseString<T> &CBaseString<T>::operator<<(const int32_t _int)
{
   checkSpace(m_length + sizeof(_int));
   int entities=(sizeof(_int)/sizeof(T));

   for(int i1=0 ; i1 < entities ; i1++)
      (*this) << (T)(_int >> ( ( entities-1 ) - i1 ) );

   return(*this);
}

template <typename T>
CBaseString<T> &CBaseString<T>::operator<<(const uint32_t _int)
{
   checkSpace(m_length + sizeof(_int));
   int entities=(sizeof(_int)/sizeof(T));

   for(int i1=0 ; i1 < entities ; i1++)
      (*this) << (T)(_int >> ( ( ( entities-1 ) - i1 ) * 8) );

   return(*this);
}
*/


template <typename T>
int &operator << (int &i, CBaseString<T> &str)
{
   int entities=(sizeof(i)/sizeof(T));

   i=0;
   for(int i1=0 ; i1 < entities ; i1++)
      i |= (int)( ((int)str.last()) << ( ( ( entities-1 ) - i1 ) * 8 ) );

   return(i);
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_STRING_HPP
