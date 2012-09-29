#ifndef LEPTO_RING_HPP
#define LEPTO_RING_HPP
/**---------------------------------------------------------------------------
 *
 * @file    ring.hpp
 * @brief   ring buffer as FIFO stack
 *
 * Just create an CRing calss from an CList which provides all functionality.
 *
 * @date   20141201
 * @author Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/list.hpp>


/*--- Defininitions --------------------------------------------------------*/


template <typename T>
class CRing: public CList<T>
{
   public:

      CRing(int maxEntries = CONFIG_LEPTO_RING_DEFAULT_SIZE)
         :CList<T>(maxEntries)
      {
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_RING_HPP
