#ifndef LEPTO_DISTANCE_HPP
#define LEPTO_DISTANCE_HPP
/**---------------------------------------------------------------------------
 *
 * @file       distance.hpp
 * @brief      Trivial function to handle overflows in counters
 *
 * Lets assume there is an package counter with 8Bit.
 * You want to check the continuity of packages and that no packages
 * are lost. E.g. you would check
 * "counter_new_package > counter_old_package". This will fail on
 * overflow border.
 * But distance( counter_old_package, counter_new_package) will be
 * correctly "1".
 *
 * @date       20250413
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <type_traits>


/*--- Definitions ----------------------------------------------------------*/


template<typename T>
constexpr T distance( T old, T _new)
{
   static_assert(std::is_signed<T>::value, "Requires signed type");
   return( _new - old );
}

static_assert( distance( (signed char)127, (signed char)-128 ) ==  1, "A" );
static_assert( distance( (signed char)-128, (signed char)127 ) ==  -1, "B" );
static_assert( distance( -1, 1 ) ==  2, "C" );
static_assert( distance( ((signed char)0x7F), ((signed char)0x80) ) ==  1, "D" );
static_assert( distance( ((signed char)0xFF), ((signed char)0x00) ) ==  1, "E" );
static_assert( distance( ((signed char)0x80), ((signed char)0x7F) ) == -1, "F" );
static_assert( distance( ((signed char)0x00), ((signed char)0xFF) ) == -1, "G" );


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_DISTANCE_HPP
