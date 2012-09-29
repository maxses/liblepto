/**---------------------------------------------------------------------------
 *
 * @file    signal.hpp
 * @brief   C++ signal handling
 *
 * Signals can be connected either to functions or to class methods.
 *
 * Example for function:
 *        CSignal<int, int> mySignal;
 *        mySignal.connect( &mySlot );
 *        mySignal.emitSignal(123);
 *
 * Example for class method:
 *        CSignal<int, int> mySignal;
 *        CMyClass myClassObject;
 *
 *    mySignal.connect( &myClassObject, &CMyClass::mySlot );
 *        mySignal.emitSignal(123);
 *
 * @date   20170127
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/signal.hpp>


/*--- Fin ------------------------------------------------------------------*/
