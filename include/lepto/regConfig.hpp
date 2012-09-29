#if ! defined (LEPTO_REG_CONFIG_HPP)
#define LEPTO_REG_CONFIG_HPP
/**---------------------------------------------------------------------------
 *
 * @file    regConfig.hpp
 * @brief   Helper class to interpret controller registers
 *
 * This class is can be used on a host PC to dump a set of register human
 * readable and to manipulate registers. The block of registers can then be
 * transfered to a microcontroller which writes them to a device.
 *
 * The target device can be radio modules for example which have a big set of
 * registers.
 *
 * @date      20231109
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#include <stdint.h>
#include <stdio.h>


/*--- Declarations ---------------------------------------------------------*/


struct SRegEntry
{
   int addr;
   int bitStart;
   int bitSize;
};

enum ESettings: unsigned int;

enum ERegType
{
   eValue,
   eEnum,
};

typedef const char* SSettingDesctriptorTexts[6];
struct SSettingDesctriptor
{
   const SRegEntry &regEntry;
   const char *regName;
   const char *descText;
   ERegType regType;
   SSettingDesctriptorTexts valueTexts;
};

#define MAX_VALUE_TEXTS    6
typedef SSettingDesctriptor SSettingDesctriptors[ ];
typedef const char** SSettingDesctriptorsI[ MAX_VALUE_TEXTS ];

extern SSettingDesctriptor* descriptors;
extern int descriptorCount;

template<typename T>
class CRegConfig
{
private:
      T* m_conf;

public:
      CRegConfig( int size )
      {
         m_conf=new T[size];
      }
      ~CRegConfig()
      {
         delete[] m_conf;
      }
      T* data()
      {
         return( m_conf );
      }
      T setConfig( const SRegEntry& regEntry, int value )
      {
         m_conf[ regEntry.addr ]=
               setBitFields(
                  m_conf[ regEntry.addr ],
                  regEntry.bitStart,
                  regEntry.bitSize,
                  value
                  );
         return( m_conf[ regEntry.addr ] );
      }
      T plainSetting( const SRegEntry& regEntry, int value )
      {
         return(
               setBitFields(
                  regEntry.bitStart,
                  regEntry.bitSize,
                  value
                  ) );
      }
      T getConfig( const SRegEntry& regEntry )
      {
         return( getBitFields(
                    m_conf[ regEntry.addr ],
                    regEntry.bitStart,
                    regEntry.bitSize
                    )
                 );
      }
      static T setBitFields( T org, T bitPos, T bitSize, T value)
      {
         T mask=(((T)-1)>>( (sizeof(T)*8) - bitSize)) << bitPos;
         return ( ( org & (~mask)) | ( (value << bitPos) & mask ) );
      }
      static constexpr T setBitFields( const T bitPos, const T bitSize, const T value)
      {
         return ( value << bitPos );
      }
      static T getBitFields( T org, T bitPos, T bitSize)
      {
         T mask=(((T)-1)>>( (sizeof(T)*8) - bitSize)) << bitPos;
         return( ( org & mask) >> bitPos );
      }

      void dumpConfig( SSettingDesctriptor *descriptors, int count )
      {
         printf("Dumping config:\n");
         for(int i1=0; i1<count; i1++)
         {
            T value=getConfig( descriptors[i1].regEntry );
            const char* desc=nullptr;
            if( value < MAX_VALUE_TEXTS )
            {
               desc=descriptors[i1].valueTexts[value];
            }

            printf("   %s: %d %s\n", descriptors[i1].descText
                   , value
                   , desc?desc:"?" );
         }
      };
};


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_REG_CONFIG_HPP
