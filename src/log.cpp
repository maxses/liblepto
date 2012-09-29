/**---------------------------------------------------------------------------
 *
 * @file    log.h
 * @brief   Logging with lepto
 *
 * Corresponding to loging functions in Qt there are following functions:
 *
 *    lInfo(msg, ...)
 *    lCaution(msg, ... )
 *    lWarning(msg, ... )
 *    lCritical(msg, ... )
 *    lFatal(msg, ... )
 *
 * These underlying functions can be optionaly overloaded. Libbiwak does this
 * for example to make logging available in ISRs.
 *
 * @date      20140107
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/lepto.h>      // IS_ENABLED
#include <lepto/log.h>
#include <cstddef>            // fix undefined size_t in debian buster
#include <lepto/ansi.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


/*--- Implementation -------------------------------------------------------*/


const char *categoryMessages[]=
#if IS_ENABLED( CONFIG_LEPTO_LOG_USE_ANSI )
   { [(int)ELogCategory::Debug]="",
     //[(int)ELogCategory::Caution]="",
     //[(int)ELogCategory::Info]="",
     [(int)ELogCategory::Caution]=ANSI_ORANGE "Caution: ",
     [(int)ELogCategory::Warning]=ANSI_ORANGE "Warning: ",
     [(int)ELogCategory::Critical]=ANSI_RED "Error: ",
     [(int)ELogCategory::Fatal]=ANSI_RED "Fatal: " };
#else
   { [ (int)ELogCategory::Debug ]="",
     [(int)ELogCategory::Caution ]="C:",
     [(int)ELogCategory::Warning ]="W:",
     [(int)ELogCategory::Critical ]="E:",
     [(int)ELogCategory::Fatal ]="F:", };
#endif

#if 0
#if defined( MCU_FLASH_SIZE )
   #if MCU_FLASH_SIZE >= 32768
      #if ! IS_ENABLED( LEPTO_LOG_USE_PRETTY_PRINT )
         #warning Your flash size is big enough. Consider enabling \
                  'LEPTO_LOG_USE_PRETTY_PRINT' and 'LEPTO_LOG_USE_ANSI'.
      #endif
   #else
      #if IS_ENABLED( LEPTO_LOG_USE_PRETTY_PRINT )
         #error Flash size: MCU_FLASH_SIZE
         #error Your flash size is quite small. Consider disabling \
               'LEPTO_LOG_USE_PRETTY_PRINT' and enabling 'LEPTO_LOG_DOWNSIZE'.
      #endif
   #endif
#else
   #warning MCU_FLASH_SIZE is not set as define.
#endif
#endif

void lVLogSimplePrint(ELogCode code, const char *format, va_list &list ) __attribute__ ( ( weak ) );
void lVLogSimplePrint(ELogCode code, const char *format, va_list &list )
{
   // Indicate that biwaks log is not used?
   //printf("lepto: ");

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

void lVLogPrettyPrint(const char*file, int line, ELogCode code, const char *format, va_list &list ) __attribute__ ( ( weak ) );
void lVLogPrettyPrint(const char*file, int line, ELogCode code, const char *format, va_list &list )
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

void lLogSimplePrint( ELogCode code, const char *format, ... )
{
   va_list list;
   va_start(list, format);
   lVLogSimplePrint( code, format, list);
   return;
}

#if IS_ENABLED( CONFIG_LEPTO_LOG_USE_PRETTY_PRINT )

void lLogPrettyPrint( const char*file, int line, ELogCode code, const char *format, ... )
{
   va_list list;
   va_start(list, format);
   lVLogPrettyPrint( file, line, code, format, list);
   return;
}

#endif

int lLogInfo( ) __attribute__ ( ( weak ) );
int lLogInfo( )
{
   return( LEPTO_CODE );
}


/*--- Fin ------------------------------------------------------------------*/
