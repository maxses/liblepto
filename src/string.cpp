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


#include <lepto/string.hpp>
#include <stdlib.h>           // malloc, free
#include <string.h>           // memcpy


/*--- Implementation -------------------------------------------------------*/


CString::~CString()
{
}

CByteArray::CByteArray()
{
}

CByteArray::~CByteArray()
{
   free(m_buf);
   m_buf=nullptr;
}

template <typename T>
CBaseString<T> &CBaseString<T>::insert(int insertPos, T _char)
{
   checkSpace(length()+1);
   if(insertPos<0)
   {
      insertPos=length()+(insertPos+1);
      // dont know what to do
      if(insertPos<0)
         return(*this);
      memmove(&(data()[insertPos+1]), &(data()[insertPos]), length()-insertPos);
      data()[insertPos]=_char;

      #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
         m_length++;
      #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
   }
   return(*this);
};


template CBaseString<char> &CBaseString<char>::insert(int pos, char _char);


/*--- Fin ------------------------------------------------------------------*/
