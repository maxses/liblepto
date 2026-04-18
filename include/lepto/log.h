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
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdarg.h>
#include <lepto/lepto.h>               // IS_ENABLED( )


/*--- Implementation -------------------------------------------------------*/


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

#define lAssert( assertion, ... ) if ( ! (assertion) ) \
         { lFatal( "ASRT" /*SW " __VA_ARGS__*/ ); };

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
   Event       = 1,  // Device-to-device logic via log codes. E.g. "I want to shut down"
   Caution     = 1,
   Warning     = 2,
   Critical    = 3,
   Fatal       = 4,

   Function    = 8,    // Function area
   Beckon      = 8,    //
   Calm        = 9,
   //Loop        = 10,
};

// The ELogCode allways contains the cathegory
enum class ELogCode: int;

inline constexpr ELogCode operator| (ELogCategory cat, int i)
{
   //return( (ELogBlended)( (int) cat | ( i << 4 ) ) );
   return( (ELogCode)( (int) cat | ( i ) ) );
}

/*
inline constexpr operator ELogBlended(ELogCategory cat)
{
   return( (ELogBlended) cat );
}
*/

inline constexpr ELogCategory toCategory(ELogCode code)
{
   return( (ELogCategory)( (int) code & 0xF ) );
}


#if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )
   #define lDebugReal(msg, ...) lLog(ELogCategory::Debug|0, msg, ##__VA_ARGS__ )
   #define lInfo(msg, ...) lLog(ELogCategory::Info|0, msg, ##__VA_ARGS__ )
   #define lCaution(msg, ... ) lLog(ELogCategory::Caution|0, msg, ##__VA_ARGS__ )
   #define lWarning(msg, ... ) lLog(ELogCategory::Warning|0, msg, ##__VA_ARGS__ )
   #define lCritical(msg, ... ) lLog(ELogCategory::Critical|0, msg, ##__VA_ARGS__ )
   #define lFatal(msg, ... ) lLog(ELogCategory::Fatal|0, msg, ##__VA_ARGS__ )
#else
   #define lDebugReal(msg, ...) lLog(ELogCategory::Debug, "" )
   #define lInfo(msg, ...) lLog(ELogCategory::Info, "" )
   #define lCaution(msg, ... ) lLog(ELogCategory::Caution, "" )
   #define lWarning(msg, ... ) lLog(ELogCategory::Warning, "" )
   #define lCritical(msg, ... ) lLog(ELogCategory::Critical, "" )
   #define lFatal(msg, ... ) lLog(ELogCategory::Fatal, "" )
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
   #define lLog( ... ) lLogPretty( __FILE_NAME__, __LINE__, __VA_ARGS__ )
#else
   #define lLog( ... ) lLogSimple( __VA_ARGS__ )
#endif

struct SLogEntry;
//__attribute__((weak))
void logCallBack( const SLogEntry *le );

extern "C"
{
   #if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )

      extern const char *categoryMessages[];
      void lVLogSimple(ELogCode code, const char *format, va_list &list );
      void lVLogPretty(const char*file, int line, ELogCode code, const char *format, va_list &list );
      void lLogSimple(ELogCode code, const char *format, ... );
      void lLogPretty(const char*file, int line, ELogCode code, const char *format, ... );
      
   #else

      void lVLog( ELogCategory category, const char *format, ... );
      void lLog( ELogCategory category, const char *format, ... );
      
   #endif
      
   void logEventLoop();
   
   // Only for unit tests
   int logPendingCount();
   const char *logPending();
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LOG_H
