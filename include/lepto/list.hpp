#ifndef LEPTO_LIST_HPP
#define LEPTO_LIST_HPP
/**---------------------------------------------------------------------------
 *
 * @file    list.hpp
 * @brief   ring buffer as FIFO stack
 *
 * The ringbuffer is a fixed array of elements with two index-pointer: 
 * back/top and front/bottom.
 * If an element is pushed, the top-pointer increases. 
 * If an element is poped/dropped, the bottom increases.
 * There must be only exist one consumer but multiple producers may exist.
 * 
 * Example:
 *    CList<int> fifo(10);
 *    fifo.push_back(0x10);
 *    fifo.push_back(0x20);
 *    printf( "Entry: 0x%X\n", fifo.pop() ); // 0x10
 *
 * Empty:                        2 Entries:
 * [0 ] [1 ] [2 ] [3 ] [4 ]      [0 ] [1*] [2*] [3 ] [4 ]
 *       |\--m_back                    |          \--m_back
 *       \-- m_front                   \-- m_front
 *
 * Full:
 * [0*] [1 ] [2*] [3*] [4*]
 *            |\--m_front = 2
 *            \-- m_back = m_front + m_maxEntries = 7
 *
 * The index pointers do not automatically get reset on overflow. 
 * They are reset when crossing a multiple of overflows.
 *
 * Usecase for class: UART buffer, Log buffer
 *
 * Configs: CONFIG_LEPTO_RING_DEFAULT_SIZE
 *          CONFIG_LEPTO_RING_SUPPORT_VOLATILE
 *             Automatically pop entries from front when buffer is full and new
 *             value is going to be pushed. To activate this behaviour, the
 *             method "setVolatile( true )" has to be called.
 *          CONFIG_LEPTO_LIST_DOWNSIZE
 *             Can safe around 380 bytes on an stm32f042 application.
 *          CONFIG_LEPTO_LIST_RESIZABLE
 *             Support automatic expanding os lists when pushing data to a
 *             full list. This can be decactivated by calling the method
 *             "setExpandable( false )".
 *
 * The behaviour is similar to QList/std::list.
 *
 * @date   20141201
 * @author Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdlib.h>        // malloc, free
#include <stdio.h>         // printf
#include <string.h>        // memset
#include <lepto/log.h>     //
#include <lepto/lepto.h>   // IS_ENABLED


/*--- Defines --------------------------------------------------------------*/


# if ! defined CONFIG_LEPTO_RING_DEFAULT_SIZE
#  define CONFIG_LEPTO_RING_DEFAULT_SIZE 0x0
#endif

#if ! defined ( CONFIG_LEPTO_RING_VITALIZE )
   #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      #define CONFIG_LEPTO_RING_VITALIZE           1
   #endif
#endif

#if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE ) && ! IS_ENABLED( CONFIG_LEPTO_RING_VITALIZE )
   #error Setting CONFIG_LEPTO_LIST_RESIZABLE but not CONFIG_LEPTO_RING_VITALIZE is not plausible.
#endif

#define CONFIG_LEPTO_LIST_INCREMENT       8
#define CONFIG_LEPTO_STRING_MEMORY_HIKE   8

#if ! defined(LEPTO_CONFIGURED)
   #error LEPTO_CONFIGURED not defined. The configuration header was probably not involved.
#endif

#if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
   // #define MOD_ENTRY                   % m_maxEntries
   // #define MOD_ENTRY_ITERATOR          % m_parent->m_maxEntries
   // #define MOD_DUPLICATED              % m_maxEntries
   // #define MOD_DUPLICATED_ITERATOR     % m_parent->m_maxEntries
   #define LEPTO_RING_SPARE_ENTRIES    1
#else
   #define MOD_ENTRY                   % m_maxEntries
   #define MOD_ENTRY_ITERATOR          % m_parent->m_maxEntries
   //#define MOD_DUPLICATED              % m_maxEntriesDuplicated
   //#define MOD_DUPLICATED_ITERATOR     % m_parent->m_maxEntriesDuplicated
   #define MOD_DUPLICATED              % ( m_maxEntries << 16 )
   #define MOD_DUPLICATED_ITERATOR     % ( m_parent->m_maxEntries << 16 )
   #define LEPTO_RING_SPARE_ENTRIES    0
#endif

#if 0
   #define DELETE_LIST delete[] m_buffers; m_buffers=nullptr;
   #define ALLOCATE_LIST m_buffers=new T[ m_maxEntries ];
#else
   #define  DELETE_LIST free( m_buffers ); m_buffers=nullptr;
   #define ALLOCATE_LIST m_buffers=(T*)malloc( sizeof(T) * m_maxEntries );
#endif


//---Definitions---------------------------------------------------------------


#if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
   // Smaller types increase
   typedef unsigned int ringIndex_t;
#else
   // Force unsigned division for cortex-m0 with no division support
   // There is no support for negative values.
   // Having it signed can cost more than 100 Bytes on miniminutnik.
   typedef unsigned int ringIndex_t;
#endif

template <typename T>
class CList
{
   friend class CString;

   private:
      T* m_buffers;
      #if ! IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
      int m_busyProducing=0;
      #endif

      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
         static constexpr int DUPLICATE_FACTOR = 0x10000; // 0x1000 was not enough for 4-thread-test
         static constexpr int DUPLICATE_SHIFT = 16; // 0x1000 was not enough for 4-thread-test
      #endif

   private:
      ringIndex_t m_frontPos;
      ringIndex_t m_backPos;
      ringIndex_t m_maxEntries;
      
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
         unsigned int m_maxEntriesDuplicated;
      #endif

      #if IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
      bool m_volatile;
      #endif // ? BIWAK_SUPPORT_VOLATILE_RING

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      bool m_resizable = true;
      #else
      #endif

   public:
#if 1
      class CIterator
      {
         private:
            const CList* m_parent;
            ringIndex_t m_pos;
         public:
            constexpr CIterator(const CList* parent, ringIndex_t pos)
                :m_parent( parent )
                ,m_pos(pos)
            {
            };
            bool operator !=(const CIterator& other) const
            {
               return( other.m_pos !=m_pos );
            }
            bool operator ==(const CIterator& other) const
            {
               return( other.m_pos == m_pos );
            }
            #if 0
            void operator =(ringIndex_t pos)
            {
               m_pos = pos;
               return;
            }
            #endif
            const T& operator *() const
            {
               return( m_parent->m_buffers[ m_parent->moduloEntry( m_pos ) ] );
            }
            CIterator& operator ++(int)
            {
               // The int is dummy and is 0;
               // its used to distinguish ++i vs i++;
               return( (*this)+=1 );
            }
            CIterator& operator +=(int number);
            CIterator& operator ++()
            {
               return( (*this)+=1 );
            }
            ringIndex_t realIndex() const
            {
               return( m_parent->realIndex( m_pos ) );
            }
            ringIndex_t index()
            {
               return( m_pos );
            }
            CIterator operator +( int add )
            {
               return( CIterator( index() + add ) );
            }
            CIterator& operator =( int pos )
            {
               m_pos=pos;
               return(*this);
            }
      };
#endif
      CList(int maxEntries = CONFIG_LEPTO_RING_DEFAULT_SIZE);
      ~CList();

      bool checkSpace(ringIndex_t newLength, bool doPreserve = true );

      #if CONFIG_LEPTO_RING_DEFAULT_SIZE == 0
      
#if 0
      /**
       * @brief Initially allocate memory when 
       */
      void vitalize();
#endif

      #endif
      
      void clear();
      bool isDataAvailableBasically() const;
      bool isDataAvailable() const;
      
      /**
       * @brief Get pointer to the entry at bottom position.
       *        This can be used before dropEntry() is called.
       *
       * @return Pointer to bottom entry or nullptr if no entries are available
       */
      T *frontEntry() const;
   
      T *getEntry(int pos) const;

      #if 1
      const CIterator at(int pos);
      #endif
      
      /**
       * @brief Get pointer to the entry at top position.
       *        This can be used before pushBack() is called.
       *
       * @return Pointer to top entry or nullptr if free entries are available
       */
      T *backEntry() const;
      
      ringIndex_t frontIndex() const
      {
         return(m_frontPos);
      }
      
      ringIndex_t backIndex() const
      {
         return(m_backPos);
      }
      
      ringIndex_t realIndex( ringIndex_t pos ) const
      {
         return( moduloEntry( pos ) );
      }
      
      constexpr ringIndex_t moduloEntry( ringIndex_t entry ) const
      {
         #if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
            // Avoid modulo operation which would cause an diivision on Cortec-M0
            #if 1
               if( entry >= m_maxEntries )
               {
                  entry-=m_maxEntries;
               }
               return(entry);
            #else
               return( entry % m_maxEntries );
            #endif
         #else
            return( entry MOD_ENTRY );
         #endif
      }
      
      constexpr ringIndex_t moduloDuplicated( ringIndex_t entry ) const
      {
         #if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
            return( moduloEntry( entry ) );
         #else
            return( entry MOD_DUPLICATED );
         #endif
      }
      
      void setIndices(ringIndex_t front, ringIndex_t back, int maxEntries, void* data)
      {
         m_frontPos = front;
         m_backPos = back;
         m_maxEntries = maxEntries;
         
         if( m_buffers )
         {
            lFatal( "TID" );
         }
         
         m_buffers = (T*)data;
         
         #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
         // Can not use huge loops in counter when super big lists are used
         // e.g. for blocks of SD card
         m_maxEntriesDuplicated = ( maxEntries *
               ( (m_maxEntries>0x10000) ? 0x4 : DUPLICATE_FACTOR ) );
         #endif
         
         return;
      }
      
      CList<T>& operator << (const T value)
      {
         push_back(value);
         return( *this );
      };

      /**
       * @brief Push an entry to the back. Thread-safe.
       */
      bool push_back_ts(const T value);
      
      /**
       * @brief Replace the last entry or push an entry to the back if list is empty
       */
      bool replaceBack(const T value);

      /**
       * @brief Push an entry to the back. Not thread-safe.
       */
      bool push_back(const T value);
      
      /**
       * @brief Pop an entry of the bottom.
       */
      T pop();

      /**
       * @brief Drop the entry at bottom position.
       */
      void dropFront();
      
      const T *putString(const T *str);
      T crosssum() const;
      T average() const;

      #if IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
      /**
       * \brief   Set volatile attribute
       *
       *          If ringbuffer is volatile it will forget older entries when
       *          new value is pushed but the buffer is already full.
       */
      void setVolatile(bool _volatile)
      {
         m_volatile=_volatile;
      }
      #endif // ? BIWAK_SUPPORT_VOLATILE_RING

      /**
       * @brief  Get the ammount of pushed/poppabel entries
       * @return Number of entries
       */
      constexpr int count() const
      {
         return( distance( m_frontPos, m_backPos ) );
      }

      int distance( CIterator front, CIterator back ) const
      {
         return( distance(front.getIndex(), back.getIndex() ) );
      }

      int distance( ringIndex_t front, ringIndex_t back ) const
      {
         // Avoid division by zero
         if( ! m_maxEntries )
         {
            return(0);
         }

         #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
         // Will not happe when CONFIG_LEPTO_LIST_DOWNSIZE is enabled
         if( back == (ringIndex_t) ( ( front + m_maxEntries ) MOD_DUPLICATED ) )
         {
            return( m_maxEntries );
         }
         #endif

         // Normalize
         front = moduloEntry( front );
         back = moduloEntry( back );

         if( front == back )
         {
            return(0);
         }

         if( front < back )
         {
            return( back - front );
         }

         return ( ( m_maxEntries - front ) + back );
      }
      
      /**
       * @brief  Get the maximum number of entries that can be stored in the ringbuffer
       *
       *         The real size which can be pushed might be smaller when
       *         CONFIG_LEPTO_LIST_DOWNSIZE is enabled.
       */
      int getMaxEntries() const
      {
         return( m_maxEntries );
      }
      
      T* getBuffers() const
      {
         return( m_buffers );
      }
      /**
       * @brief  Get the number of entries that can pushed till ringbuffer is full
       */
      int getFreeCount() const
      {
         return( getMaxEntries() - count() );
      }

      /**
       * @brief  Dump the internal attributes to the terminal. For debugging only
       */
      void dump() const;

      /**
       * @brief  Push the entry on the top index
       */
      void pushBack()
      {
         lAssert( pushable() );

         ringIndex_t nextBack=moduloDuplicated( m_backPos + 1 );

         m_backPos = nextBack;
      }

      /**
       * @brief check if value can be pushed to buffer without resizing
       */
      bool pushable() const
      {
         return ( ! isFull( m_frontPos, m_backPos) );
      }
      
      /**
       * @brief   Reserve an entry that can be pushed later.
       *          The entry can be published by "pushReserved()" later. Do not
       *          take to much time till pushing because till then the buffer
       *          is block at the index. But other threads can already reserve
       *          entries for them before this one is pushed.
       * @return  Index of the reserved element or '-1' if buffer is full
       */
      ringIndex_t tryReserve()
      {
         bool valid;
         ringIndex_t nextBack;
         ringIndex_t reserved;
         
         #if ( CONFIG_LEPTO_RING_DEFAULT_SIZE == 0 ) && IS_ENABLED( CONFIG_LEPTO_RING_VITALIZE )
            /*
            if( m_maxEntries == 0 )
            {
               vitalize();
            }
            */
         #endif
         

         #if ! IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
            __atomic_add_fetch(&m_busyProducing, 1, __ATOMIC_SEQ_CST);
         #endif

         do{
            reserved=m_backPos;
            ringIndex_t front=m_frontPos;

            if( isFull(front, reserved) )
            {
               #if ! IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
                  __atomic_sub_fetch(&m_busyProducing, 1, __ATOMIC_SEQ_CST);
               #endif

               return(-1);
            }
            nextBack=moduloDuplicated( reserved + 1 );
            valid=__atomic_compare_exchange_n( &m_backPos, &reserved, nextBack,
                           true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST );
         }while(!valid);

         return( reserved );
      }
      
      /**
       * @brief   Push an previously reserved entry
       */
      void pushReserved( ringIndex_t index )
      {
         (void)index;

         #if ! IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
            __atomic_sub_fetch( &m_busyProducing, 1, __ATOMIC_SEQ_CST);
         #endif
      }
      
      /**
       * @brief   Check if the buffer is full and no further entries can be pushed
       */
      bool isFull(ringIndex_t front, ringIndex_t back) const
      {
         //#if CONFIG_LEPTO_RING_DEFAULT_SIZE == 0
         if( m_maxEntries == 0)
         {
            return( true );
         }
         //#endif
         
         #if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
            return(  moduloEntry( back + LEPTO_RING_SPARE_ENTRIES ) ==  front );
         #else

            #if 0
            // Cornercase: When back-position is at the very end and the front is
            // at the beginning, adding an additional entry must not be allowed.
            // Stack would become "empty" but all entries are marked 'valid'.
            // How can this happen?
            if( ( back == m_maxEntriesDuplicated -1 ) && ! front )
            {
               return( true );
            }
            #endif

            return(  back == (ringIndex_t) ( ( front + m_maxEntries ) MOD_DUPLICATED ) );
         #endif
      }
      
      /**
       * @brief   Check if the buffer is full and no further entries can be pushed
       */
      bool isFull() const
      {
         // Avoid division by zero via modulo 0
         if( !m_maxEntries )
         {
            return( true );
         }
         return(  isFull( m_frontPos, m_backPos ) );
      }
      
      /**
       * @brief   Get pointer to an previously reserved entry
       */
      T* reservedEntry(ringIndex_t index) const
      {
         if( index == (ringIndex_t)-1 )
         {
            return( nullptr );
         }

         return( &m_buffers[ moduloEntry( index ) ] );
      }
      
      /**
       * @brief   Just for internal testing
       */
      void setFrontBack( ringIndex_t front, ringIndex_t back, int maxEntries=0)
      {
         // Must set maxEntries first to let MOD_DUPLICATED work
         if( maxEntries )
         {
            if( m_buffers )
            {
               // Can not change entry count when memory is already alocated
               lFatal( "CNCE" );
               // This is only usefull on "pseudo" lists without own data
            }
            m_maxEntries = maxEntries;
            
            #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
               m_maxEntriesDuplicated = maxEntries * DUPLICATE_FACTOR;
            #endif
         }
         m_frontPos=moduloDuplicated( front );
         m_backPos=moduloDuplicated( back );
      }
      
      /**
       * @brief   Just for internal testing
       */
      int getMaxEntriesDuplicated()
      {
         #if IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
            return( m_maxEntries );
         #else
            return( m_maxEntriesDuplicated );
         #endif
      }
      
      CIterator begin()
      {
         return( CIterator( this, m_frontPos ) );
      }
      
      CIterator end()
      {
         return( CIterator( this, m_backPos ) );
      }

      const CIterator begin() const
      {
         return( CIterator( this, m_frontPos ) );
      }

      const CIterator end() const
      {
         return( CIterator( this, m_backPos ) );
      }
      static void left(CIterator& front, CIterator& mid, CIterator& back)
      {
         back=mid;
      }
      static void right(CIterator& front, CIterator& mid, CIterator& back)
      {
         front=mid;
      }

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )

      /**
       * @brief Set weather the list can be resized automatically or not
       *
       * E.g. Interrupt service routines must not allocate memory. Buffers sohuld
       * not be allowed to automatically resize.
       *
       * @param resizable true: List will get resized if necessary
       */
      void setResizable( bool resizable )
      {
         m_resizable = resizable;
      }

      #endif

      /**
       * @brief Find element in list.
       *
       * The returned iterator points to the element that is equal or bigger
       * than the wanted.
       * The operator '<' for comparing has to be implemented seperately.
       * e.g. "bool operator <(CList<bool>::CIterator i1, const int i2)"
       * By using th eiterator the list does not need to contain the data itself
       * (m_buffers==nullptr).
       *
       * @param element  Element to be find.
       * @return  Iterator to element.
       */
      template< typename C >
      CIterator find(const C& element);

   protected:

      /**
       * @brief Direct reference to entry
       *        This is only used in inherited classes.
       * @param pos: Real index to entry
       * @return reference
       */
      T &rawEntry(ringIndex_t pos) const
      {
         lAssert( pos <= m_maxEntries );
         return( m_buffers[ moduloEntry( pos ) ] );
      }

      #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      bool expand( void );
      #endif

      void allocate( int size );

};


template <typename T>
CList<T>::CList(int maxEntries)
   :m_frontPos(0)
   ,m_backPos(0)
   ,m_maxEntries(maxEntries + LEPTO_RING_SPARE_ENTRIES)
   #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
   ,m_maxEntriesDuplicated( ( maxEntries + LEPTO_RING_SPARE_ENTRIES ) * DUPLICATE_FACTOR )
   #endif
   #if IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
   ,m_volatile(false)
   #endif
{
   if( m_maxEntries )
   {
      allocate( m_maxEntries );
      lFullAssert( m_buffers != nullptr );
   }
   else
   {
      m_buffers = nullptr;
   }

   #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
   // Don't mess with the duplicated range
   lFullAssert( m_maxEntries < (0x7FFFFFFF / DUPLICATE_FACTOR ) );
   #endif
   
   return;
};


template <typename T>
CList<T>::~CList()
{
   if( m_buffers )
   {
      DELETE_LIST
   }

   return;
};


template <typename T>
bool CList<T>::checkSpace(ringIndex_t newSize, bool doPreserve /*=true*/ )
{
   #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      T *oldBuf=m_buffers;
      ringIndex_t oldSize=count();
      ringIndex_t oldMaxSize=m_maxEntries;
   #endif

   // We need size+1 to keep a zero
   if( newSize + LEPTO_RING_SPARE_ENTRIES > m_maxEntries )
   {
      #if ! IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
         return( false );
      #else
         if( !m_resizable )
         {
            return(false);
         }

         newSize = MAX( newSize + LEPTO_RING_SPARE_ENTRIES, oldSize + CONFIG_LEPTO_STRING_MEMORY_HIKE );
         allocate( newSize );

         m_backPos=0;
         if( doPreserve )
         {
            #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
               m_length=oldLength;
            #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH
            if( oldBuf )
            {
               for(ringIndex_t i1=0; i1<oldSize; i1++)
               {
                  m_buffers[i1]=oldBuf[( i1 + m_frontPos ) % oldMaxSize];
               }
               m_backPos=oldSize;
            }
            /*
            else
            {
               m_buffers[0]=0;
            }
            */
         } // ? preserve
         else
         {
            #if IS_ENABLED( CONFIG_LEPTO_STRING_CACHED_LENGTH )
            m_length=0;
            #endif // CONFIG_LEPTO_STRING_CACHED_LENGTH

            // m_buffers[0]=0;
         }
         m_frontPos=0;

         if( oldBuf )
         {
            free(oldBuf);
         }
      #endif
   }

   return true;
}


template <typename T>
void CList<T>::clear()
{
   m_frontPos=m_backPos=0;
}


template <typename T>
T CList<T>::pop()
{
   T value{0};

   if( isDataAvailable() )
   {
      value=*frontEntry();
      dropFront();
   }

   return(value);
};


template <typename T>
T *CList<T>::frontEntry() const
{
   T *value=nullptr;

   // Has to be checked manually.
   if( isDataAvailableBasically() )
   {
      value=&m_buffers[ moduloEntry( m_frontPos ) ];
   }

   return(value);
};


template <typename T>
T *CList<T>::backEntry() const
{
   T *value=nullptr;

   // Don't edit the top of a full buffer: Its also the bottom
   if( ! isFull() )
   {
      value=&m_buffers[ moduloEntry( m_backPos ) ];
   }

   return(value);
};


template <typename T>
void CList<T>::dropFront()
{
   if(isDataAvailableBasically())
   {
      m_frontPos = moduloDuplicated( m_frontPos + 1 );
   }
   else
   {
      lFatal( LDS("NE", "No Entry") );
   }

   return;
};


template <typename T>
bool CList<T>::push_back_ts(const T value)
{
   // To use threadsafe pushing, list must not be expandable by definition

   #if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )
      lAssert( m_resizable == false );
   #endif

   #if IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
   if( isFull() && m_volatile )
   {
      dropFront();
   }
   #endif // ? #if BIWAK_SUPPORT_VOLATILE_RING
   
   ringIndex_t index=tryReserve();
   
   if( index == (ringIndex_t)-1 )
   {
      #if IS_ENABLED( CONFIG_LEPTO_LIST_ABORT_FAILING_PUSH )
         abort();
      #endif
      return(false);
   }

   *reservedEntry(index)=value;
   pushReserved( index );

   return(true);
};


template <typename T>
bool CList<T>::push_back(const T value)
{

#if IS_ENABLED( CONFIG_LEPTO_RING_SUPPORT_VOLATILE )
   if( isFull() && m_volatile )
   {
      dropFront();
   }
#endif // ? #if BIWAK_SUPPORT_VOLATILE_RING
   
   checkSpace( count() + 1 );

   T* back=backEntry();
   
   if( !back )
   {
      #if IS_ENABLED( CONFIG_LEPTO_LIST_ABORT_FAILING_PUSH )
         abort();
      #endif
      return(false);
   }
   *back=value;
   pushBack( );
   
   return(true);
};

template <typename T>
bool CList<T>::replaceBack(const T value)
{
    if( ! count() )
    {
        return( push_back(value) );
    }
    else
    {
        m_buffers[ moduloEntry( m_backPos + ( m_maxEntries - 1 ) ) ] = value;
    }
    return(true);
}


template <typename T>
T CList<T>::crosssum() const
{
   T value=0;
   int pos=0;
   int entryCount;
   if(! ( entryCount = count() ) )
   {
      return(value);
   }

   for(int cnt=0; cnt< entryCount; cnt++)
   {
      value += *getEntry(pos);
      pos++;
   }

   return( value );
}


template <typename T>
T CList<T>::average() const
{
   T value=crosssum();
   
   // in case there are no values at all, avoid division by zero.
   if( !value )
   {
      return(value);
   }
   
   return( value / count() );
}


/**
 * @brief   Does the buffer contain pushed data, ignoring locks
 *
 *          This has to be used when locks shall be ignored. E.g.
 *          frontElement() should not return a nullpointer. The applicaton
 *          already checked for valid data but m_busyProducing could change
 *          afterwards.
 * @return  true: Data available; false: No data available
 */
template <typename T>
bool CList<T>::isDataAvailableBasically() const
{
   return( m_backPos != m_frontPos );
}


template <typename T>
bool CList<T>::isDataAvailable() const
{
   return( isDataAvailableBasically()
        #if ! IS_ENABLED( CONFIG_LEPTO_RING_NO_THREADS )
           && ( m_busyProducing == 0 )
         #endif
      );
}


template <typename T>
T *CList<T>::getEntry(int pos) const
{
   T *value=nullptr;

   if( pos < count() )
   {
      // Pos is relative to start
      pos= moduloEntry( pos + m_frontPos );
      value=&(m_buffers[ pos ]);
   }

   return(value);
}

#if 1

template <typename T>
const typename CList<T>::CIterator CList<T>::at(int pos)
{
   return( CList<T>::CIterator(this, moduloDuplicated( pos + m_frontPos ) ) );
}

#endif

template <typename T>
const T *CList<T>::putString(const T *str)
{
   for(int i1=0; str[i1]; i1++)
      putValue(str[i1]);

   return(str);
};


template <typename T>
typename CList<T>::CIterator& CList<T>::CIterator::operator +=(int number)
{
   lDebugAssert( number );
   m_pos = m_parent->moduloDuplicated( m_pos + number );
   return( *this );
};

#if IS_ENABLED( CONFIG_LEPTO_LIST_RESIZABLE )

template <typename T>
bool CList<T>::expand( )
{
   if( !m_resizable )
   {
      return(false);
   }

   T *oldBuffers=m_buffers;
   int size=count();
   m_maxEntries+= CONFIG_LEPTO_LIST_INCREMENT;
   ALLOCATE_LIST;

   lFullAssert( m_buffers != nullptr );

   for(int i1=0; i1<size; i1++)
   {
      m_buffers[i1]=oldBuffers[ (i1+m_frontPos) MOD_ENTRY ];
   }

   free( oldBuffers );
   //delete[](oldBuffers);

   m_frontPos=0;
   m_backPos=size;

   return(true);
};

#endif // ? CONFIG_LEPTO_LIST_RESIZABLE


template <typename T>
void CList<T>::allocate( int size )
{
   m_maxEntries = size;
   ALLOCATE_LIST;

   #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
   m_maxEntriesDuplicated= m_maxEntries * DUPLICATE_FACTOR;
   #endif
}


template <typename T> template<typename C>
typename CList<T>::CIterator CList<T>::find(const C& candidate)
{
   CIterator front( this, m_frontPos );
   CIterator back( this, m_backPos );
   CIterator mid(this, 0);
   #if 0
   int steps=0;
   #endif

   while( distance(front, back) > 1 )
   {
      mid=moduloEntry( front.getIndex() +(distance( front.getIndex(), back.getIndex() ) /2 ) );
      //printf("   Check: Front: 0x%X; Back: 0x%X; distance: %d; value: 0x%X\n", front.getIndex(), back.getIndex(), distance(front, back), candidate );
      if( mid < candidate )
      {
         right(front, mid, back);
      }
      else
      {
         left(front, mid, back);
      }
      #if 0
      steps++;
      if(steps>1000)
      {
         lFatal("");
      }
      #endif
   }

   // printf("   Front: 0x%X; Back: 0x%X; distance: %d; value: 0x%X\n", front.getIndex(), back.getIndex(), distance(front, back), candidate );
   if( front < candidate )
   {
      //printf("   F\n");
      return( back );
   }

   //printf("   B\n");
   return( front );
}

// Who knows... sometime someone might want to debug. Some tests actually use this
#if HOST || __x86_64__

template <typename T>
void CList<T>::dump() const
{
   printf("m_frontPos: %d / %d\n", m_frontPos, m_frontPos % m_maxEntries);
   printf("m_backPos: %d / %d\n", m_backPos, m_backPos % m_maxEntries);
   printf("m_maxEntries: %d\n", m_maxEntries);
   #if ! IS_ENABLED( CONFIG_LEPTO_LIST_DOWNSIZE )
      printf("m_maxEntriesDuplicated: %d\n", m_maxEntriesDuplicated);
   #endif
   
   // This only makes sense when <T> is an ineger. But it could be an structure
   // and compiler will throw warning even it is not used.
   #if 0
   for(int i1=0; i1<m_maxEntries; i1++)
   {
      printf( "   [%02X]: %d\n", i1, m_buffers[i1] );
   }
   #endif
}

#endif // ? HOST || __x86_64__


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LIST_HPP
