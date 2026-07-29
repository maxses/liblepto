#ifndef LEPTO_CRTP_HPP
#define LEPTO_CRTP_HPP
/**---------------------------------------------------------------------------
 *
 * @file       crtp.hpp
 * @brief      Class for "Curiously Recurring Template Pattern"
 *
 * With the help of CRTP an interface class can be realized without using
 * virtual functions. This reduces overhead. The polimorphism is
 * done at compile time instead of runtime.
 *
 * This is usefull when you are desperately want to save some bytes on very
 * small bare metal systems.
 *
 * Warning: Don't use it when you do not really need it. You will loose dynamic
 * polymorphism. Binary size can even increase when multiple instances are
 * created.
 *
 * The base class is a template class which gets the derived class as template
 * parameter ("curiously recurring").
 *
 * Example:
 *      template <class Derived>
 *      class CBase: public CCrtpBase<Derived>
 *      {
 *          public:
 *              int setDefaults()
 *              {
 *                  printf("Setting defaults; Base\n");
 *                  CRTP_CALL( setDefaults_cb );
 *                  return(0);
 *              }
 *
 *      };
 *
 *      class CCustom: public CBase<CCustom>
 *      {
 *          public:
 *              int setDefaults_cb()
 *              {
 *                  printf("Setting defaults; Derived\n");
 *                  return(0);
 *              }
 *      };
 *
 * @date       20260720
 * @author     Maximilian Seesslen <src@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Declaration ----------------------------------------------------------*/


#define CRTP_CAST(name)      \
    this->derived().name

#define CRTP_CALL(name, args...)      \
    this->derived().name( args  )


template<class Derived>
class CCrtpBase
{
   public:

       //template<class Derived>
       Derived &derived()
       {
          return static_cast<Derived &>(*this);
       };

       //template<class Derived>
       const Derived &derived() const
       {
          return static_cast<const Derived &>(*this);
       };

   public:

       /* Example:

           void callback()
           {
              CRTP_CALL ( callback_cb, 0, 1 );
           }

       */
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_CRTP_HPP
