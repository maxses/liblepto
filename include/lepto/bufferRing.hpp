#ifndef LEPTO_BUFFER_RING_HPP
#define LEPTO_BUFFER_RING_HPP
/**---------------------------------------------------------------------------
 *
 * @file       bufferRing.hpp
 * @brief      Buffer ring; a variant of CRing
 *
 * There must be only exist one producer and one consumer at the same
 * time
 *
 * Empty:                        2 Entries:
 * [0 ] [1*] [2 ] [3 ] [4 ]      [0 ] [1*] [2*] [3 ] [4 ]
 *       |    \--m_top                 |         \--m_top
 *       \-- m_bottom                  \-- m_bottom
 *
 * Full:
 * [0*] [1*] [2*] [3*] [4*]
 *       |    \--m_top
 *       \-- m_bottom
 *
 * This class is used in biwak usb class.
 *
 * @date       20221222
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/ring.hpp>
//#include <biwak/timer.h>


/*--- Implementation -------------------------------------------------------*/


struct SBuffer{
   //bool used;
   int size;
   void *data;
   // lrtimer_t timestamp;
};

class CBufferRing: public CRing<SBuffer>
{
   private:
#if 0
      struct SBuffer{
         bool used;
         int size;
         void *data;
      };
      SBuffer  *m_buffers;
      int m_bufferSize;
      int m_bufferCount;

      //int m_topBuffer;
      //int m_bottomBuffer;
      int m_currentBuffer;
      int m_usedBuffer;
#endif
      int m_maxBufferSize;

   public:

      CBufferRing(int bufferSize, int buffers)
         :CRing(buffers)
         ,m_maxBufferSize(bufferSize)
         //,m_bufferCount(buffers)
         //,m_bottomBuffer(0)
         //,m_topBuffer(0)
         //,m_currentBuffer(0)
         //,m_usedBuffer(0)
      {
         //m_buffers=new SBuffer[buffers];


         for(int i1=0; i1<buffers; i1++)
         {
            SBuffer &buffer=rawEntry(i1);
            //buffer.used=0;
            buffer.data=malloc(bufferSize);
            buffer.size=0;
         }
      }
      int getMaxBufferSize() const
      {
         return(m_maxBufferSize);
      }
      // exceptionally push an buffer first and then fill it with data
      void *pushBuffer( int size )
      {
         SBuffer *buffer=backEntry();

         lDebugAssert( size <= m_maxBufferSize );

         if(!buffer)
         {
            return(nullptr);
         }
         buffer->size=size;
         // buffer->timestamp=lrNow();
         pushBack();
         return( buffer->data );
      }
      void *getTopData() const
      {
         /*
         if( ! backEntry() )
         {
            return(nullptr);
         }
         */
         return( backEntry()->data );
      }
      void *getBottomData() const
      {
         if( frontEntry() )
         {
            return( frontEntry()->data );
         }
         else
         {
            return(nullptr);
         }
      }
      int getBottomSize() const
      {
         lDebugAssert( frontEntry() );
         return(  frontEntry()->size);
      }
      void dropBuffer()
      {
         //lDebugAssert( m_bottomPos >= 0 );
         dropFront();
      }
      int usedBuffers() const
      {
         return( count() );
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_BUFFER_RING_HPP
