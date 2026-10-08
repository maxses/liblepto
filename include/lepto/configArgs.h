#ifndef LEPTO_CONFIG_ARGS_H
#define LEPTO_CONFIG_ARGS_H
/**--------------------------------------------------------------------------
 *
 * @file       configArgs.h
 * @brief      Simple macro to handle configuration via predefine
 *
 *             The macro "IS_ENABLED()" can be used to check if some feature is
 *             enabled by predefine.
 *             This way an application can use some default setting when an
 *             define is not set but just setting a define (to nothing) still
 *             enables a feature.
 *
 *             Example:
 *                gcc main.cpp -DCONFIG_FEATURE     # Feature is explicitly enabled
 *                gcc main.cpp                      # Feature is not set but main.cpp could enable it
 *                gcc main.cpp -DCONFIG_FEATURE=0   # Feature is explicitly disabled
 *                gcc main.cpp -DCONFIG_FEATURE=Off # Feature is explicitly disabled
 *                gcc main.cpp -DCONFIG_FEATURE=y   # Feature is explicitly enabled
 *
 * @date       20250801
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *---------------------------------------------------------------------------*/

#define CONFIGARG_y     1
#define CONFIGARG_Y     1
#define CONFIGARG_YES   1
#define CONFIGARG_Yes   1
#define CONFIGARG_yes   1
#define CONFIGARG_true  1
#define CONFIGARG_1     1
#define CONFIGARG_n     0
#define CONFIGARG_N     0
#define CONFIGARG_NO    0
#define CONFIGARG_No    0
#define CONFIGARG_no    0
#define CONFIGARG_ON    1
#define CONFIGARG_On    1
#define CONFIGARG_on    1
#define CONFIGARG_OFF   0
#define CONFIGARG_Off   0
#define CONFIGARG_off   0
#define CONFIGARG_false 0
#define CONFIGARG_0     0
#define CONFIGARG_      1

#define LEPTO_IS_ENABLED_IMPL(name) CONFIGARG_##name
#define IS_ENABLED(name) LEPTO_IS_ENABLED_IMPL(name)

#endif // ! LEPTO_CONFIG_ARGS_H
