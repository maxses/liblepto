#ifndef LEPTO_PRESET_CONFIG_HOST_H
#define LEPTO_PRESET_CONFIG_HOST_H
//----------------------------------------------------------------------------
///
/// \brief  Config preset for compiling for host
///
///         Enable features that are required in the unit tests.
///
/// \date   20250905
/// \author Maximilian Seesslen <src@seesslen.net>
///
//----------------------------------------------------------------------------


#if defined LEPTO_LOG_USE_PRETTY_PRINT
   #error "LEPTO_LOG_USE_PRETTY_PRINT" is obsolete.
#endif

#if ! defined ( STM32 )
   #undef CONFIG_LEPTO_LOG_DOWNSIZE
   #undef CONFIG_LEPTO_LOG_PRETTY_PRINT
   #define CONFIG_LEPTO_LOG_PRETTY_PRINT           1
   #define CONFIG_LEPTO_RING_SUPPORT_VOLATILE      1

   // #undef CONFIG_LEPTO_LIST_RESIZABLE
   // #define CONFIG_LEPTO_LIST_RESIZABLE             1

   // e.g. Fosh examples will use CApplication and can not be build without this
   #undef CONFIG_LEPTO_GLOBAL_EVENT_LOOP
   #define CONFIG_LEPTO_GLOBAL_EVENT_LOOP          1

#endif


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_PRESET_CONFIG_HOST_H
