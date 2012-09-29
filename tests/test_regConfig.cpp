/**---------------------------------------------------------------------------
 *
 * @file       test_base64.cpp
 * @brief      Test base64 encoding and decoding
 *
 * @date       20220619
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#if defined ( CATCH_V3 )
#include <catch2/catch_test_macros.hpp>
#elif defined ( CATCH_V2 )
#include <catch2/catch.hpp>
#elif defined ( CATCH_V1 )
#include <catch/catch.hpp>
#else
#error "Either 'catch' or 'catch2' has to be installed"
#endif

#include <lepto/regConfig.hpp>

#define TEST_ALL
#define STOP_ON_FAIL


/*--- Implementation -------------------------------------------------------*/


enum ERegs: int
{
   RegFifo          = 0x0,
   RegOpMode        = 0x1,
};

constexpr const SRegEntry RESequencerOff{ RegOpMode, 7, 1 };
namespace ESequencerOff {
   enum ESequencerOff: int {
      SequencerOn= 0x0,
      SequencerOff= 0x1,
   };
}

constexpr const SRegEntry REListenOn{ RegOpMode, 6, 1 };
namespace EListenOn {
   enum EListenOn: int {
      ListenOn= 0x1,
      ListenOff= 0x0,
   };
}


SSettingDesctriptors _descriptors=
{
   //------ RegOpMode
    { RESequencerOff, "RESequencerOff", "Sequencer Off", ERegType::eEnum, {
               [ESequencerOff::SequencerOn]="Sequencer on",
               [ESequencerOff::SequencerOff]="Sequencer Off" } },
   { REListenOn, "REListenOn", "Listening On", ERegType::eEnum, {
               [EListenOn::ListenOff]="Listen Off",
               [EListenOn::ListenOn]="Listen On" } },
};

TEST_CASE( "regConfig", "[default]" )
{
   SECTION( "regCOnfig" )
   {
      CRegConfig<uint8_t> regConfig(0x80);
      regConfig.data()[1]=0x80;

      REQUIRE( regConfig.getConfig( RESequencerOff ) == ESequencerOff::SequencerOff );
      REQUIRE( regConfig.getConfig( REListenOn ) == EListenOn::ListenOff );

      regConfig.data()[1]=0x40;
      REQUIRE( regConfig.getConfig( RESequencerOff ) == ESequencerOff::SequencerOn );
      REQUIRE( regConfig.getConfig( REListenOn ) == EListenOn::ListenOn );

      regConfig.dumpConfig( _descriptors, 2);
   }
}


/*--- Fin ------------------------------------------------------------------*/
