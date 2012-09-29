/**---------------------------------------------------------------------------
 *
 * @file       crc8.cpp
 * @brief      Functions for CRC8 calculation in software
 *
 * The CRC8 setting is "CRC-8/NRSC-5"
 *
 * @date       20211127
 * @author     Maximilian Seesslen <mes@seesslen.net>
 * @copyright  SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <stdint.h>
#include <stddef.h>                    // size_t


/*--- Implementation -------------------------------------------------------*/


uint8_t crc8Init()
{
   return( 0xff );
}


uint8_t crc8Calc(uint8_t crc, void const *mem, size_t len)
{
    uint8_t const *data = (uint8_t const *) mem;
    if (data == NULL)
        return 0xff;
    while (len--) {
        crc ^= *data++;
        for (unsigned k = 0; k < 8; k++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc;
}


/*--- Fin ------------------------------------------------------------------*/
