#ifndef LEPTO_LOG_PRINTER_H
#define LEPTO_LOG_PRINTER_H
/**---------------------------------------------------------------------------
 *
 * @file    log_printer.h
 * @brief   Functions actually print logs
 *
 * Should not be used directly. See log.h for loging mechanisms.
 *
 * @date      20260416
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Includes -------------------------------------------------------------*/


#include <lepto/log_private.h>


/*--- Implementation -------------------------------------------------------*/


void logPrintSimple( const SLogEntry* le );
void logPrintPretty( const SLogEntry* le );


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_LOG_PRINTER_H
