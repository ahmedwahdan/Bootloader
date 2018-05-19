
 #include "bootloader.h"
 #include <avr/io.h>
 #include "Flash_Driver.h"


 int main(void)
 {
	 
	 DDRA&=~(1<<2);
	 PORTA |=(1U<<0);
	 DDRD|=(1<<7) |(1<<6);
	 
	 Ptr2Fun startApp   = (Ptr2Fun) 0x0000;		 /**< Pointer to application at flash start */ 
	  
	 while(1)
	 {	
		 
		if((PINA&(1<<0)) && (Read_Mem_Byte(0x0000) != 0xFF))
		{
			  PORTD|=(1<<6);
			  startApp();
			
		}
		else
		{
			 PORTD|=(1<<7);
			 Bootloader_Init();
			 Bootloader_Start();
		}
		
	 }
	 
 }
 
 

 