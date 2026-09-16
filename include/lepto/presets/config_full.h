#ifndef LEPTO_PRESET_CONFIG_FULL_H
#define LEPTO_PRESET_CONFIG_FULL_H
/**---------------------------------------------------------------------------
 *
 * @file    config_full.h
 * @brief   Config defines for full feature devices
 *
 * @date   20250202
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


// Obsolete:
// #define CONFIG_LEPTO_LOG_SIGNAL           1

#define CONFIG_LEPTO_LIST_RESIZABLE          1
#define CONFIG_LEPTO_LOG_PRETTY_PRINT        1
#define CONFIG_LEPTO_LOG_ANSI_COLORS         1
#define CONFIG_LEPTO_LOG_CALLBACK            1
#define CONFIG_LEPTO_RING_SUPPORT_VOLATILE   1
#define CONFIG_LEPTO_SIGNAL_CHAIN            1
#define CONFIG_LEPTO_SIGNAL_FUNCTION         1
#define CONFIG_LEPTO_SIGNAL_METHOD           1

#if ! defined( STM32 )

   #define CONFIG_LEPTO_RING_SUPPORT_VOLATILE   1

#endif


//---fin-----------------------------------------------------------------------
#endif // ? ! LEPTO_PRESET_CONFIG_FULL_H
