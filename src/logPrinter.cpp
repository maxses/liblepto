/**---------------------------------------------------------------------------
 *
 * @file    logPrinter.cpp
 * @brief   Functions actually print logs
 *
 * Should not be used directly. See log.h for loging mechanisms.
 *
 * @date      20260416
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>      // IS_ENABLED
#include <lepto/log.h>
#include <lepto/log_private.h>
#include <cstddef>            // fix undefined size_t in debian buster
#include <lepto/ansi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <lepto/ring.hpp>


/*--- Implementation -------------------------------------------------------*/


#if defined CONFIG_LEPTO_LOG_USE_TIMESTAMPS
   #error CONFIG_LEPTO_LOG_USE_TIMESTAMPS is obsolete. Use CONFIG_LEPTO_LOG_TIMESTAMPS
#endif

#if defined CONFIG_LEPTO_LOG_USE_PRETTY_PRINT
#error CONFIG_LEPTO_LOG_USE_PRETTY_PRINT is obsolete. Use CONFIG_LEPTO_LOG_PRETTY_PRINT
#endif

#if defined CONFIG_LEPTO_LOG_USE_ANSI
#error CONFIG_LEPTO_LOG_USE_ANSI is obsolete. Use CONFIG_LEPTO_LOG_USE_ANSI_COLORS
#endif

//#if defined CONFIG_BIWAK_LOG_HEARTBEAT
//#error CONFIG_BIWAK_LOG_HEARTBEAT is obsolete. Use CONFIG_LEPTO_LOG_HEARTBEAT
//#endif

#if defined CONFIG_LEPTO_LOG_HEARTBEAT
#error CONFIG_LEPTO_LOG_HEARTBEAT is invalid.
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT ) || IS_ENABLED( CONFIG_LEPTO_LOG_USE_ANSI )
#include <lepto/ansi.h>
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT ) && IS_ENABLED( CONFIG_LEPTO_LOG_DOWNSIZE )
#error Having LEPTO_LOG_DOWNSIZE and LEPTO_LOG_USE_PRETTY_PRINT enabled at \
the same time does not make sense.
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_USE_ANSI )
#endif

const char *categoryMessages[]=
#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
   { [(int)ELogCategory::Debug]="",
     //[(int)ELogCategory::Caution]="",
     //[(int)ELogCategory::Info]="",
     [(int)ELogCategory::Caution]=ANSI_YELLOW "Caution: ",
     [(int)ELogCategory::Warning]=ANSI_ORANGE "Warning: ",
     [(int)ELogCategory::Critical]=ANSI_RED "Error: ",
     [(int)ELogCategory::Fatal]=ANSI_RED "Fatal: " };
#else
    asd
   { [ (int)ELogCategory::Debug ]="",
     [(int)ELogCategory::Caution ]="C:",
     [(int)ELogCategory::Warning ]="W:",
     [(int)ELogCategory::Critical ]="E:",
     [(int)ELogCategory::Fatal ]="F:", };
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )
void logPrintSimple( const SLogEntry* le )
{
   return;
}

void logPrintPretty( const SLogEntry* le )
{
   return;
}

#elif IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )

void logPrintPretty( const SLogEntry* le )
{
   #if IS_ENABLED( CONFIG_LEPTO_LOG_TIMESTAMPS )
      printf("[%08d] ",  le->timeStamp );
   #endif

   int len=printf("%s(%d)" , le->file, le->line);
   for(int i1=0; i1<22-len; i1++)
   {
      fputs(".", stdout);
   }
   fputs( ": ", stdout );
   fputs( categoryMessages[ (int)toCategory(le->code) ], stdout );
   fputs( ANSI_NORMAL,  stdout );
   fputs( le->logString, stdout );
   fputs( "\n", stdout );
}
   
#else

void logPrintSimple( const SLogEntry* le );
{
   fputs( categoryMessages[ (int)toCategory(le->code) ], stdout );
   // fputs does not print an newline
   fputs(le->logString, stdout );
   fputs("\n", stdout );
}


#endif // ? CONFIG_LEPTO_LOG_PRETTY_PRINT ELSE

/*--- Fin ------------------------------------------------------------------*/
