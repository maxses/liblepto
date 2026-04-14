#ifndef LEPTO_DEBUG_HPP
#define LEPTO_DEBUG_HPP
/**---------------------------------------------------------------------------
 *
 * @file       debug.hpp
 * @brief      Helper class for debuging
 *
 * Can check access of deleted objects.
 *
 * @date       20260406
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/log.h>


/*--- Declaration ----------------------------------------------------------*/


class CDebug
{
   private:
      int m_instanceState=0;

   public:
      CDebug()
      {
         m_instanceState=1;
      }
      ~CDebug()
      {
         m_instanceState=2;
      }
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_DEBUG_HPP
