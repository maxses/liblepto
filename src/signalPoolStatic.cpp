/**---------------------------------------------------------------------------
 *
 * @file    signalPoolStatic.hpp
 * @brief   Pool for C++ signals
 *
 * Signals can be enqueued into a list and at a different time point
 * (e.g. event loop) the connected slots are called.
 *
 * Unlike 'CPendingSignalPool' this pool does not create objects for each
 * signal. This pool only supports methods without any argument.
 * The benefit is that no malloc functions are used. Therefore it can be feeded
 * from ISR.
 *
 * @date   20170127
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/signalPoolStatic.hpp.h>


/*--- Implementation -------------------------------------------------------*/


static CPendingSignalPoolStatic* CPendingSignalPoolStatic::gsp=nullptr;


/*--- Fin ------------------------------------------------------------------*/
