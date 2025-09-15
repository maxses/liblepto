#ifndef LEPTO_ANSI_H
#define LEPTO_ANSI_H
/**---------------------------------------------------------------------------
 *
 * @file       ansi.h
 * @brief      Macros for ansi sequences
 *
 * @date      20140107
 * @author    Maximilian Seesslen <mes@seesslen.net>
 * @copyright SPDX-License-Identifier: Apache-2.0
 *
 *--------------------------------------------------------------------------*/


/*--- Defines --------------------------------------------------------------*/


#define ESC "\033"

/*
 //0x: Spezial Attribut(Blink, intensive, ...)
 //3x: Set forground x
 //4x: Set Background x
 //9x: Set Foreground bright x

 // test with:
 for(int i1=0;i1<10;i1++)
 {
         for(int i2=0;i2<010;i2++)
         {       printf(ESC "[00m");
                 printf(ESC "[37m");
                 printf(ESC "[%d%dm %d-%d ", i1,i2,i1,i2);
         };
         printf("\n");
 }
*/
#define ANSI_RED ESC "[31m"
#define ANSI_GREEN ESC "[32m"
#define ANSI_MAGENTA ESC "[35m"
#define ANSI_BLACK ESC "[30m"
#define ANSI_BLUE ESC "[34m"
#define ANSI_BROWN ESC "[33m"
#define ANSI_xx1 ESC "[36m"             // Tuerkis
#define ANSI_GREY ESC "[37m"
#define ANSI_ORANGE ANSI_BROWN
#define ANSI_BLACK ESC "[30m"
#define ANSI_NORMAL ESC "[39m"
#define ANSI_COL_FORMAT ESC "[3%dm"

#define ANSI_FONT_BOLD ESC "[1m"                // Fett
#define ANSI_FONT_BRIGHT ESC "[1m"              // Fett
#define ANSI_FONT_NORMAL ESC "[0m"              // Fett
#define ANSI_FONT_UNDERLINE ESC "[4m"   // Unterstrichen
#define ANSI_FONT_BLINK ESC "[5m"               // Unterstrichen

#define ANSI_CLEARSCREEN ESC "[2J"
#define ANSI_RESET ESC "[0m"
#define ANSI_HOME ESC "[H"
#define ANSI_DELETE ESC "[D"


/*--- Fin ------------------------------------------------------------------*/
#endif // ? ! LEPTO_ANSI_H
