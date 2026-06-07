#ifndef LEPTO_PRESET_CONFIG_SMALL_DEVICE_H
#define LEPTO_PRESET_CONFIG_SMALL_DEVICE_H
//----------------------------------------------------------------------------
///
/// \brief  Config preset for small devices
///
///         -
//
/// \date   20250202
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


// Shrink it very much. There are only have 16KB for application

#define CONFIG_LEPTO_LOG_DOWNSIZE                  1
#define CONFIG_LEPTO_LOG_NO_USE_ANSI               1
#define CONFIG_LEPTO_LOG_NO_TEXT                   0
#define CONFIG_LEPTO_LOG_PRETTY_PRINT              0
#define CONFIG_LEPTO_RING_DOWNSIZE                 1


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_PRESET_CONFIG_SMALL_DEVICE_H
