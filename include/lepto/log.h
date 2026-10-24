#ifndef LEPTO_LOG_H
#define LEPTO_LOG_H
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
 * @author    Maximilian Seesslen <src@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdarg.h>
#include <lepto/lepto.h>               // IS_ENABLED( )
#include <stdlib.h>                    // abort()
#include <lepto/logCodes.h>


/*--- Implementation -------------------------------------------------------*/


#if ! defined ( CONFIG_LEPTO_LOG_CALLBACK )
   #define CONFIG_LEPTO_LOG_CALLBACK               1
#endif

// #if ! defined LEPTO_LOG_NO_USE_ANSI
#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
   #if ! IS_ENABLED( CONFIG_LEPTO_LOG_ANSI_COLORS )
      #define CONFIG_LEPTO_LOG_ANSI_COLORS
   #endif
#endif

#if defined LEPTO_LOG_DEBUG
   #define lDebug( ... ) lDebugReal( __VA_ARGS__ )
#else
   #define lDebug(a,...)
#endif

#if defined LEPTO_LOG_DEBUG || defined LEPTO_LOG_DEBUG_ASSERT || USE_FULL_ASSERT
   #define lDebugAssert(a, ...) if(! (a)){ lFatal("Assertion false: '" #a "'. " __VA_ARGS__ ); }
#else
   #define lDebugAssert(a, ...)
#endif

#if defined ( STM32 )
   #define lAssert( assertion, ... ) if ( ! (assertion) ) \
         { abort(); };
   #define lHostAssert( assertion, ... )
#else
   #define lAssert( assertion, ... ) if ( ! (assertion) ) \
      { throw("Assertion wrong: " #assertion); };
   #define lHostAssert( assertion, ... ) if ( ! (assertion) ) \
      { throw("Assertion wrong: " #assertion); };
#endif

#define lAssertIndexInArray(_enum, _array) \
         lDebugAssert( (_enum) < (sizeof(_array) / sizeof(_array[0]) ), _enum )

#if USE_FULL_ASSERT
   #define lFullAssert( assertion, ... ) if ( ! (assertion) ) \
      { lFatal( "ASRT" /*SW " __VA_ARGS__*/ ); };
#else
   #define lFullAssert( assertion, ... )
#endif


enum class ELogCategory
{
   Debug       = 0,
   Good        = 0,
   Info        = 0,
   Gone        = 1,
   Event       = 2,  // Device-to-device logic via log codes. E.g. "I want to shut down"
   Caution     = 2,
   Warning     = 3,
   Critical    = 4,
   Fatal       = 5,

   Function    = 8,    // Function area
   Beckon      = 8,    //
   Calm        = 9,
   //Loop        = 10,
};

// The ELogCode allways contains the cathegory
#if 0
enum class ELogCode: int;

inline constexpr ELogCode operator| (ELogCategory cat, int i)
{
   //return( (ELogBlended)( (int) cat | ( i << 4 ) ) );
   return( (ELogCode)( (int) cat | ( i ) ) );
}
#else
   //enum class ELogCode: int;
   typedef int ELogCode;
#endif
/*
inline constexpr operator ELogBlended(ELogCategory cat)
{
   return( (ELogBlended) cat );
}
*/

#if 0
inline constexpr ELogCategory toCategory(ELogCode code)
{
   return( (ELogCategory)( (int) code & 0xF ) );
}
#endif

#if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )
   #define lDebugReal(msg, ...) lLog(ELogCategory::Debug, msg, ##__VA_ARGS__ )
   #define lInfo(msg, ...) lLog(ELogCategory::Info, msg, ##__VA_ARGS__ )
   #define lCaution(msg, ... ) lLog(ELogCategory::Caution, msg, ##__VA_ARGS__ )
   #define lWarning(msg, ... ) lLog(ELogCategory::Warning, msg, ##__VA_ARGS__ )
   #define lCritical(msg, ... ) lLog(ELogCategory::Critical, msg, ##__VA_ARGS__ )
   
   #if IS_ENABLED( CONFIG_LEPTO_LOG_CODE )
      #define lWarningC( code, msg, ... ) lLog(ELogCategory::Warning, "#%X " msg, code, ##__VA_ARGS__ )
      #define lCriticalC( code, msg, ... ) lLog(ELogCategory::Critical, "#%X """ msg, code, ##__VA_ARGS__ )
   #else
      #define lWarningC( code, msg, ... ) lLog(ELogCategory::Warning, msg, ##__VA_ARGS__ )
      #define lCriticalC( code, msg, ... ) lLog(ELogCategory::Critical, msg, ##__VA_ARGS__ )
   #endif
#if defined STM32
      #define lFatal(msg, ... ) abort();
   #else
      #define lFatal(msg, ... ) lLog(ELogCategory::Fatal, msg, ##__VA_ARGS__ )
   #endif
   #define lCalm() lLog(ELogCategory::Calm|0, "" )
#else
   #define lDebugReal(msg, ...) lLog(ELogCategory::Debug|0, "" )
   #define lInfo(msg, ...) lLog(ELogCategory::Info|0, "" )
   #define lCaution(msg, ... ) lLog(ELogCategory::Caution|0, "" )
   #define lWarning(msg, ... ) lLog(ELogCategory::Warning|0, "" )
   #define lCritical(msg, ... ) lLog(ELogCategory::Critical|0, "" )
   #define lFatal(msg, ... ) lLog(ELogCategory::Fatal|0, "" )
   #define lCalm() lLog(ELogCategory::Calm|0, "" )

   #define lWarningC(code, msg, ... ) lLog(ELogCategory::Warning|0, "" )
   #define lCriticalC(code, msg, ... ) lLog(ELogCategory::Critical|0, "" )
#endif

// You can use the LDS macro to give two different messages depending on the
// flash size. Example: lCritical( LDS ("DSBL DEV 0x%X", "Disabling device 0x%X"), dev);
#if IS_ENABLED( CONFIG_LEPTO_LOG_DOWNSIZE )
   #define LDS( msgShort, msgDetailed ) msgShort
#else
   #define LDS( msgShort, msgDetailed ) msgDetailed
#endif

// The CMake helpers only shows the compilation unit. This is confusing. The
// problem only exists on old distributions. Containing full pathes is not
// very critical as long as STM32 toolchains have proper __FILE_NAME__.
#if 0
inline constexpr
const char* shrinkFileName( const char* const filename )
{
   return( __builtin_strrchr(filename, '/') + 1 );
};
#endif

#if ! defined __FILE_NAME__
   #if defined ( STM32 )
      // THe workaround in the CMakeLists.
      // #warning "__FILE_NAME__ is not defined. Implement it in CMake to save memory."
      // #warning "You may want to add 'find_package(Lepto REQUIRED)' to your CMakeLists.txt"
      #define __FILE_NAME__ "unknown"
   #else
      #define __FILE_NAME__ __builtin_strrchr( __FILE__, '/') + 1
   #endif
#endif

#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
   #if IS_ENABLED( CONFIG_LEPTO_LOG_DIRECT_PRINT )
      #define lLog( ... ) lLogPretty( __FILE_NAME__, __LINE__, __VA_ARGS__ ); \
               logEventLoop();
   #else
      #define lLog( ... ) lLogPretty( __FILE_NAME__, __LINE__, __VA_ARGS__ )
   #endif
#else
   #define lLog( ... ) lLogSimple( __VA_ARGS__ )
#endif

struct SLogEntry;

extern "C"
{
   void logCallBack( const SLogEntry *le );
   void logCallBack2( const SLogEntry *le );

#if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )

      extern const char *categoryMessages[];
      void lVLogSimple(ELogCategory code, const char *format, va_list &list );
      void lVLogPretty(const char*file, int line, ELogCategory code, const char *format, va_list &list );
      void lLogSimple(ELogCategory code, const char *format, ... );
      void lLogPretty(const char*file, int line, ELogCategory code, const char *format, ... );
      
   #else

      void lVLog( ELogCode code, const char *format, ... );
      void lLog( ELogCode code, const char *format, ... );
      
   #endif
      
   void logEventLoop();
   
   // Only for unit tests
   int logPendingCount();
   const char *logPending();
   void leptoInitLog();
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LOG_H
