#ifndef LEPTO_LOG_PRIVATE_H
#define LEPTO_LOG_PRIVATE_H
//----------------------------------------------------------------------------
///
/// \brief  Logging
///
//
/// \date   20221222
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//---Includes-----------------------------------------------------------------


//---Own------------------------------

#include <lepto/log.h>         // ELogCategory
#include <lepto/lepto.h>       // defaults CONFIG_LEPTO_LOG_PRETTY_PRINT
#if IS_ENABLED( CONFIG_LEPTO_LOG_TIMESTAMPS )
//#include <biwak/timer.hpp>
#include <time.h>
#endif


//---Defines------------------------------------------------------------------


// At least unit tests need bigger strings
#if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
   // Less than 40 cuts many logs
   #define CONFIG_LEPTO_LOG_MAX_STRING_LENGTH 40
   #define CONFIG_LEPTO_LOG_MAX_ENTRIES 24
#else
   #if CONFIG_LEPTO_LOG_LENGTH
      #define MAX_LOG_STRING CONFIG_LEPTO_LOG_LENGTH
   #else
      #define MAX_LOG_STRING 20
   #endif
   // Entries: 4 not enough for mainswitch, 7 not for miniminutnik
   #define MAX_LOG_ENTRIES 8

   // #define MAX_LOG_STRING 80
   // Entries: 4 not enough for mainswitch, 7 not for miniminutnik
   // #define MAX_LOG_ENTRIES 8
#endif

#if ! defined CONFIG_LEPTO_LOG_PRETTY_PRINT
   #if ! defined CONFIG_LEPTO_LOG_DOWNSIZE
      #error LEPTO_LOG_USE_PRETTY_PRINT is not defined but LEPTO_LOG_DOWNSIZE \
             also not. This will lead to cut messages when using biwak.
   #endif
#endif


//---Declaration--------------------------------------------------------------


#if ! IS_ENABLED( CONFIG_LEPTO_LOG_DIRECT_PRINT )

struct SLogEntry
{
   #if IS_ENABLED( CONFIG_LEPTO_LOG_PRETTY_PRINT )
      const char *file;
      int line;
   #endif
      
   #if IS_ENABLED( CONFIG_LEPTO_LOG_TIMESTAMPS )
      clock_t timeStamp;
   #endif
      
   ELogCode code;

   #if IS_ENABLED( CONFIG_LEPTO_LOG_EVENTS )
   //unsigned int event;
   #endif

   #if ! IS_ENABLED( CONFIG_LEPTO_LOG_SILENT )
   char logString[ CONFIG_LEPTO_LOG_MAX_STRING_LENGTH ];
   #endif
};

#endif // ! CONFIG_LEPTO_LOG_DIRECT_PRINT


//---fin----------------------------------------------------------------------
#endif // ? ! LEPTO_LOG_PRIVATE_H
