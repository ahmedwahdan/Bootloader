/*
 * bootloader.h
 *
 * Created: 3/6/2017 6:35:34 PM
 *  Author: Kareem A. Abullah
 */ 


#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdint.h>

#define  TERMINATING_CHARACTER			'\r'



typedef void (*Ptr2Fun)( void );



 void Bootloader_Init(void);
  void Bootloader_Start(void);


void Bootloader_UART_Receiver(uint8_t data);


#endif /* BOOTLOADER_H_ */