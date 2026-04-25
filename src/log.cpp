/**---------------------------------------------------------------------------
 *
 * @file    log.h
 * @brief   Logging with lepto
 *
 * Logging functions analog to Qt.
 * There are following basic functions:
 *
 *    lInfo(msg, ...)
 *    lCaution(msg, ... )
 *    lWarning(msg, ... )
 *    lCritical(msg, ... )
 *    lFatal(msg, ... )
 *
 * @date      20140107
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>      // IS_ENABLED
#include <lepto/log.h>
#include <lepto/log_private.h>
#include <lepto/logPrinter.h>
#include <cstddef>            // fix undefined size_t in debian buster
#include <lepto/ansi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <lepto/ring.hpp>

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT ) || IS_ENABLED( CONFIG_LEPTO_LOG_ANSI_COLORS )
#include <lepto/ansi.h>
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_TIMESTAMPS )
#include<sys/time.h>
#endif


/*--- Implementation -------------------------------------------------------*/


#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT ) && IS_ENABLED( CONFIG_LEPTO_LOG_DOWNSIZE )
#error Having LEPTO_LOG_DOWNSIZE and LEPTO_LOG_USE_PRETTY_PRINT enabled at \
the same time does not make sense.
#endif

#if ! IS_ENABLED( CONFIG_LEPTO_LOG_DIRECT_PRINT )
   
   #if 1
   static CRing<SLogEntry> logs( CONFIG_LEPTO_LOG_MAX_ENTRIES );
   void leptoInitLog()
   {
   }
   #else
   // This is 40 bytes bigger on 'rufa'
   #define logs (*logsPtr)
   CRing<SLogEntry>* logsPtr=nullptr;
   void biwakInitLog()
   {
      logsPtr=new CRing<SLogEntry>( CONFIG_LEPTO_LOG_MAX_ENTRIES );
   }
   #endif

#endif // ? ! CONFIG_LEPTO_LOG_DIRECT_PRINT

static bool logOverflow=false;


#if IS_ENABLED( CONFIG_LEPTO_LOG_DIRECT_PRINT )

void lVLogSimple(ELogCode code, const char *format, va_list &list )
{
   fputs( categoryMessages[ (int)( toCategory(code) ) ], stdout );

   #if IS_ENABLED( CONFIG_LEPTO_LOG_USE_ANSI )
      fputs( ANSI_NORMAL, stdout );
   #endif

   vprintf( format, list );
   fputs( "\n", stdout );

   switch( toCategory(code) )
   {
      case ELogCategory::Fatal:
         #if defined ( STM32 )
            abort();
         #else
            throw("Assertion wrong");
         #endif
         break;
      default:
         break;
   }

   return;
}

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )

void lVLogPretty(const char*file, int line, ELogCode code, const char *format, va_list &list )
{
   printf("%s(%d): ", file, line);
   fputs( categoryMessages[ (int)toCategory( code ) ], stdout );
   #if IS_ENABLED( CONFIG_LEPTO_LOG_USE_ANSI )
      fputs( ANSI_NORMAL, stdout );
   #endif

   vprintf( format, list );
   fputs( "\n", stdout );

   switch( toCategory( code ) )
   {
      case ELogCategory::Fatal:
         #if defined ( STM32 )
            abort();
         #else
         throw( (int)code );
         #endif
         break;
      default:
         break;
   }

   return;
}

#endif // ? CONFIG_LEPTO_LOG_PRETTY_PRINT

#else  // ? CONFIG_LEPTO_LOG_DIRECT_PRINT ELSE

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
void lVLogPretty(const char*file, int line, ELogCode code, const char *format, va_list &list )
#else
void lVLogSimple(ELogCode code, const char *format, va_list &list )
#endif
{
   // Can not produce any more messages? Remeber to add log on next event loop
   if(!logs.pushable())
   {
      logOverflow=true;
      return;
   }
   int leIndex=logs.tryReserve();
   SLogEntry *le=(SLogEntry *)logs.reservedEntry( leIndex );
   
   if(!le)
   {
      // Avoid exception. When can this happen?
      return;
   }
   
   #if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
      le->file=file;
      le->line=line;
      #if IS_ENABLED( CONFIG_LEPTO_LOG_TIMESTAMPS )
         /*
         struct timeval tv;
         gettimeofday(&tv,NULL);
         le->timeStamp = ( ( (long long)tv.tv_sec)*1000 )+(tv.tv_usec/1000);
         */
         le->timeStamp = clock() / 1000;
      #endif
   #endif
      
   #if IS_ENABLED( CONFIG_LEPTO_LOG_CALLBACK )
      le->code=code;
   #endif
      
   #if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )
      vsnprintf( &le->logString[0], CONFIG_LEPTO_LOG_MAX_STRING_LENGTH, format, list);
   #endif
   
   if( toCategory(code) == ELogCategory::Fatal )
   {
      #if defined ( STM32 )
         // Fatal means fatal. Dont even try to print anything.
         abort();
      #else
         printf("FATAL: %s\n", le->logString);
         *((int*)0)=1;
         throw("Assertion wrong");
      #endif
   }
   
   logs.pushReserved( leIndex );
   
   return;
}

#endif // ? CONFIG_LEPTO_LOG_DIRECT_PRINT ELSE

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )

void lLogPretty( const char*file, int line, ELogCode code, const char *format, ... )
{
   va_list list;
   va_start(list, format); // expected-note{{Initialized va_list}}
   lVLogPretty( file, line, code, format, list);
   return;
}

#else

void lLogSimple( ELogCode code, const char *format, ... )
{
   va_list list;
   va_start(list, format);
   lVLogSimple( code, format, list);
   return;
}

#endif // ? CONFIG_LEPTO_LOG_PRETTY_PRINT ELSE

#if IS_ENABLED( CONFIG_LEPTO_LOG_SIGNAL )
CSignal<void, ELogCode> m_signalLog;
CSignal<void, ELogCode>& signalLog()
{
   return( m_signalLog );
};
#endif

void logEventLoop()
{
   const SLogEntry* le;
   while( ( le = (const SLogEntry*)logs.frontEntry() ) )
   {
      // Dont crash due to missing category string. Text is empty anyways.
      if( (int)toCategory(le->code) < (int)ELogCategory::Function )
      {
      #if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
         logPrintPretty(le);
      #else
         logPrintSimple(le);
      #endif
      }
   #if IS_ENABLED( CONFIG_LEPTO_LOG_CALLBACK )
      logCallBack( le );
   #endif
      
   #if IS_ENABLED( CONFIG_LEPTO_LOG_SIGNAL )
      m_signalLog.emitSignal( le->code );
   #endif // ? CONFIG_BIWAK_LOG_SIGNAL
      logs.dropFront();
   }
   
   if(logOverflow)
   {
      logOverflow=false;
      lCritical( "LOF" );
   }
}


#if IS_ENABLED( CONFIG_LEPTO_LOG_CALLBACK )

__attribute__(( weak ))
void logCallBack( const SLogEntry *le )
{
   // Just test if strong functions was used
   // printf("DLCB\n");
};

#endif


/*--- Fin ------------------------------------------------------------------*/
