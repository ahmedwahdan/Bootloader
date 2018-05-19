
 #include "uart_driver.h"
 #include "Flash_Driver.h"
 #include "srec_parser.h"
 #include "bootloader.h"
 #include "Global_Config.h"



 Ptr2Fun startApp   = (Ptr2Fun) 0x0000;			/*!< Pointer to application at flash start */
 Ptr2Fun startBoot  = (Ptr2Fun)0x3800;			/*!< Pointer to the Bootloader start address */

 
  uint8_t g_Recieve_Buffer[MEM_PAGE_SIZE];
  volatile uint8_t g_Is_Receive_Buffer_Full;
  volatile uint8_t g_Buffer_Fill_Level;

 void Bootloader_UART_Receiver(uint8_t data)
 {
	 static uint8_t s_Buffer_Index = 0;
	 
	 //entering the terminating character or reaching the page size, triggers flash write.
	 if ((data == TERMINATING_CHARACTER) || (s_Buffer_Index >= (SPM_PAGESIZE - 1)))
	 {
		 g_Is_Receive_Buffer_Full = TRUE;
		 g_Buffer_Fill_Level = s_Buffer_Index;
		 s_Buffer_Index = 0;
		 
	 }
	 else
	 {
		 g_Recieve_Buffer[s_Buffer_Index++] = data;
	 }
 }

 void Bootloader_Init(void)
 {
	 
	Disable_Interrupts();
	UART_Init(19200,Bootloader_UART_Receiver);
	
	// move interrupt vector table to boot loader area
	GICR =(1<<IVCE);
	GICR =(1<<IVSEL);
	
	Enable_Interrupts();
	
 }


 void Bootloader_Start()
 {
	 uint8_t result = 0;
	 UART_Write_Byte('S');
	 while(1)
	 {
		 
		 if(g_Is_Receive_Buffer_Full)
		 {
			 
			 Disable_Interrupts();  
			 result = Srec_Process_Record(g_Recieve_Buffer);
			
			 if(result == RECORD_OK)
			 {
				 UART_Write_Byte(RECORD_OK);
			 }
			 
			 else if(result == RECORD_TERMINATION)
			 {
				
				 UART_Write_Byte(RECORD_OK);
				 Wait_for_Transmission_Complete();
				 UART_DeInit();
				 Disable_Interrupts();
				 GICR = (1U<<IVCE);
				 GICR = 0x00;
				 startApp();       
			 }
			 else
			 {
				 UART_Write_Byte(result);
			 }
			 g_Is_Receive_Buffer_Full = FALSE;  
			 Enable_Interrupts();  
		 }
	 }

 }