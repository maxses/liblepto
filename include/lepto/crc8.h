#ifndef LEPTO_CRC8_H
#define LEPTO_CRC8_H
/**---------------------------------------------------------------------------
 *
 * @file       crc8.h
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


#include <stdint.h>                    // uint8_t
#include <stddef.h>


/*--- Declaration ----------------------------------------------------------*/


uint8_t crc8Init();
uint8_t crc8Calc(uint8_t crc, void const *mem, size_t len);


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_CRC8_H
