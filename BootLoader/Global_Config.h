/*
******************************************
******************************************
**            COMMON_CONFIG.h           **
**            Date: 2/18/2017           **
**      MCU: AVR ATmega32/ATmega32L     **
**         IDE: Atmel Studio 6.2        **
**           Compiler: AVR-GCC          **
**       Author: Mohamed M. Hesham      **
**   E-mail: moh.moh.hesham@gmail.com   **
******************************************
******************************************
*/
#ifndef GLOBAL_CONFIG_H_
#define GLOBAL_CONFIG_H_

#include <avr/interrupt.h>

#define SYSTEM_FREQ	8000000ULL


#define  Disable_Interrupts()		cli()
#define  Enable_Interrupts()		sei()

#endif /* GLOBAL_CONFIG_H_ */