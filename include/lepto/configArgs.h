#ifndef LEPTO_CONFIG_ARGS_H
#define LEPTO_CONFIG_ARGS_H
/**---------------------------------------------------------------------------
 *
 * @file       configArgs.h
 * @brief      Simple macro to handle configuration via predefine
 *
 *             The macro "IS_ENABLED()" can be used to check if some feature is
 *             enabled by predefine.
 *             This way an application can use some default setting when an
 *             define is not set but just setting an define (to nothing) still
 *             enables an feature.
 *
 *             Example:
 *                gcc main.cpp -DCONFIG_FEATURE     # Feature is explicitly enabled
 *                gcc main.cpp                      # Feature is not set but main.cpp could enable it
 *                gcc main.cpp -DCONFIG_FEATURE=0   # Feature is explicitly disabled
 *                gcc main.cpp -DCONFIG_FEATURE=Off # Feature is explicitly disabled
 *                gcc main.cpp -DCONFIG_FEATURE=y   # Feature is explicitly enabled
 *
 * @date       20250801
 * @author     Maximilian Seesslen <mes@seesslen.net>
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
#define CONFIGARG_N     0      // CMIS code uses 'N' as variable
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
#define CONFIGARG_1     1
#define CONFIGARG_      1

#define S_IS_ENABLED(a) CONFIGARG_##a
#define IS_ENABLED(a) S_IS_ENABLED( a )


#define CONFIGARG_TEST_ENABLED
#define CONFIGARG_TEST_ENABLED_1     1
#define CONFIGARG_TEST_DISABLED_0    0
#define CONFIGARG_TEST_DISABLED_n    n
#define CONFIGARG_TEST_ENABLED_ON    ON
#define CONFIGARG_TEST_DISABLED_OFF  OFF

#if ! IS_ENABLED( CONFIGARG_TEST_ENABLED )
   #error IS_ENABLED does not work [1]
#endif

#if ! IS_ENABLED( CONFIGARG_TEST_ENABLED_1 )
   #error IS_ENABLED does not work [2]
#endif

#if IS_ENABLED( CONFIGARG_TEST_DISABLED )
   #error IS_ENABLED does not work [3]
#endif

#if IS_ENABLED( CONFIGARG_TEST_DISABLED_0 )
   #error IS_ENABLED does not work [3]
#endif

#if IS_ENABLED( CONFIGARG_TEST_DISABLED_n )
   #error IS_ENABLED does not work [4]
#endif

#if ! IS_ENABLED( CONFIGARG_TEST_ENABLED_ON )
   #error IS_ENABLED does not work [5]
#endif

#if IS_ENABLED( CONFIGARG_TEST_DISABLED_OFF )
   #error IS_ENABLED does not work [6]
#endif


/*--- Fin -------------------------------------------------------------------*/
#endif // ? ! LEPTO_CONFIG_ARGS_H
