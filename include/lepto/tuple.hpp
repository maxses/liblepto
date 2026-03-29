#ifndef LEPTO_TUPLE_HPP
#define LEPTO_TUPLE_HPP
/**---------------------------------------------------------------------------
 *
 * @file    tupel.hpp
 * @brief   Struct for tupels
 *
 * This is needed for defered signals. Variadic template arguments can be
 * stored in a tuple struct.
 *
 * @date   20260319
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Declarations ---------------------------------------------------------*/


template<typename T> struct remove_reference      { using type = T; };
template<typename T> struct remove_reference<T&>  { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };

template<typename T>
T&& doForward(typename remove_reference<T>::type& t) {
    return static_cast<T&&>(t);
}

template<typename... Ts>
struct STuple;

template<>
struct STuple<> {};

template<typename T, typename... Ts>
struct STuple<T, Ts...>
{
    T head;
    STuple<Ts...> tail;
    STuple() = default;
    STuple(T&& h, Ts&&... ts)
        : head(doForward<T>(h)), tail(doForward<Ts>(ts)...) {}
};

// No remaining arguments-> call method
template<typename Obj, typename Method, typename... Collected>
void callMethodWithTuple_impl(
   Obj* obj,
   Method m,
   const STuple<>&,
   Collected&&... collected )
{
   (obj->*m)(doForward<Collected>(collected)...);
}

// Add head recursively
template<typename Obj, typename Method, typename T, typename... Ts, typename... Collected>
void callMethodWithTuple_impl(
   Obj* obj,
   Method m,
   const STuple<T, Ts...>& t,
   Collected&&... collected )
{
   callMethodWithTuple_impl(
      obj,
      m,
      t.tail,
      doForward<Collected>(collected)...,
      t.head
   );
}

// Public function
template<typename Obj, typename Method, typename... Ts>
void callMethodWithTuple(Obj* obj, Method m, const STuple<Ts...>& t)
{
   callMethodWithTuple_impl(obj, m, t);
}


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_TUPLE_HPP
