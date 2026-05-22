#ifndef LEPTO_PRESET_CONFIG_FULL_H
#define LEPTO_PRESET_CONFIG_FULL_H
//----------------------------------------------------------------------------
///
/// \brief  Config for host device
///
///         -
//
/// \date   20250202
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


// Obsolete:
// #define CONFIG_LEPTO_LOG_SIGNAL           1

#define CONFIG_LEPTO_LOG_PRETTY_PRINT     1
#define CONFIG_LEPTO_LOG_ANSI_COLORS      1
#define CONFIG_LEPTO_SIGNAL_CHAIN         1
#define CONFIG_LEPTO_SIGNAL_FUNCTION      1
#define CONFIG_LEPTO_SIGNAL_METHOD        1
#define CONFIG_LEPTO_LOG_CALLBACK         1

#if ! defined( STM32 )

   #define CONFIG_LEPTO_RING_SUPPORT_VOLATILE   1

#endif


#define LEPTO_CONFIGURED                  1


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_PRESET_CONFIG_FULL_H
