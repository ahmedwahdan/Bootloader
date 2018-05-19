
 #include "srec_parser.h"
 #include "Flash_Driver.h"
 #include "uart_driver.h"
 
 #define DEBUG_ENABLE (0)
 
 static uint8_t Mem_Page_To_Write = 0x0000;
 volatile static char received_Record_Counter = 0;	

uint8_t AsciiNibbleToNumber(uint8_t nibble)
{
	uint8_t nibble_Number = 0;
	if(nibble <= '9')
	{
		nibble_Number = nibble - '0';
	}
	else
	{
		nibble_Number = nibble - 'A' + 10;
	}
	
	return nibble_Number ;
}


uint8_t AsciiByteToNumber(uint8_t high_nibble, uint8_t low_nibble)
{
	uint8_t high_Nibble_Number = 0;
	uint8_t low_Nibble_Number = 0;
	uint8_t number = 0;
	high_Nibble_Number = (AsciiNibbleToNumber(high_nibble) << 4);
	low_Nibble_Number  =  AsciiNibbleToNumber(low_nibble);
	
	number = high_Nibble_Number + low_Nibble_Number;
	return number;
}

 uint8_t Srec_Process_Record(uint8_t Srec_Record_Buff[])
 {
	static uint8_t page_Buffer_Fill_Level = 0;
	 Srec_record_t s_record;
	 uint8_t page_Buffer[128];
	 uint8_t high_Nibble_temp = 0;
	 uint8_t low_Nibble_temp  = 0;
	 uint8_t temp_byte = 0;
	
	 uint8_t result = RECORD_OK;

	 s_record.Record_Header = Srec_Record_Buff[SREC_HEADER];
	 s_record.Record_Type	= Srec_Record_Buff[SREC_TYPE];
	 Srec_Record_Buff+=2;
	 if(s_record.Record_Header == 'S' )
	 {
		 
		 if((s_record.Record_Type == SREC_TWO_BYTE_ADDRESS))
		 {
			 high_Nibble_temp = Srec_Record_Buff[0];
			 low_Nibble_temp  = Srec_Record_Buff[1];
			 Srec_Record_Buff+=2;
			 temp_byte = AsciiByteToNumber(high_Nibble_temp, low_Nibble_temp);
			 s_record.ByteCount = temp_byte;
			 #if DEBUG_ENABLE
			 UART_Write_String("ByteCount: ");
			 UART_Write_Byte(high_Nibble_temp);
			 UART_Write_Byte(low_Nibble_temp);
			 #endif
			 if(s_record.ByteCount <= 19)
			 {
				
				s_record.Checksum = s_record.ByteCount;
				s_record.Address  = 0x0000;
				
				uint16_t address_index = 0;
				#if DEBUG_ENABLE
				UART_Write_String("Address: ");
				#endif
				for(address_index = 0; address_index < 2; address_index++)
				{
					high_Nibble_temp = Srec_Record_Buff[0];
					low_Nibble_temp =  Srec_Record_Buff[1];
					Srec_Record_Buff+=2;
					#if DEBUG_ENABLE
					UART_Write_Byte(high_Nibble_temp);
					UART_Write_Byte(low_Nibble_temp);
					#endif
					temp_byte = AsciiByteToNumber(high_Nibble_temp, low_Nibble_temp);
					s_record.Address <<= 8;
					s_record.Address|= temp_byte;
					s_record.Checksum+= temp_byte;
				}
				uint8_t data_index = 0;
				#if DEBUG_ENABLE
				UART_Write_String(" data: ");
				#endif
				for(data_index = 0; data_index < ((s_record.ByteCount- CHECKSUM_NUMBER_OF_BYTES - ADDRESS_NUMBER_OF_BYTES)); data_index++)
				{
					high_Nibble_temp =  Srec_Record_Buff[0];
					low_Nibble_temp  =  Srec_Record_Buff[1];
					Srec_Record_Buff+=2;
					#if DEBUG_ENABLE
					UART_Write_Byte(high_Nibble_temp);
					UART_Write_Byte(low_Nibble_temp);
					#endif
					temp_byte = AsciiByteToNumber(high_Nibble_temp, low_Nibble_temp);
					s_record.Checksum += temp_byte;
					page_Buffer[page_Buffer_Fill_Level+data_index] = temp_byte;
				}
				
				page_Buffer_Fill_Level +=(data_index);
				received_Record_Counter++;
								
				high_Nibble_temp = Srec_Record_Buff[0];
				low_Nibble_temp  = Srec_Record_Buff[1];
				#if DEBUG_ENABLE
				UART_Write_String(" Check sum: ");
				UART_Write_Byte(high_Nibble_temp);
				UART_Write_Byte(low_Nibble_temp);
				UART_Write_Byte('\r');
				UART_Write_Byte('\n');
				#endif
				s_record.Checksum += AsciiByteToNumber(high_Nibble_temp, low_Nibble_temp);
				
				
				if(s_record.Checksum != 0xFF)
				{
					
					result =  RECORD_CHECK_SUM_ERROR;
				}
				 
			 }
			 else
			 {
				 result = RECORD_SIZE_ERROR;
			 }
			 
			 
			 
		 }
		 else if ((s_record.Record_Type == SREC_HEADER_RECORD) || (s_record.Record_Type == SREC_TERMINATION_RECORD))
		 {
		 }
		 else
		 {
			result = RECORD_TYPE_ERROR;
		 }
		
		 if((s_record.Record_Type == SREC_TERMINATION_RECORD))
		 {
			 result = RECORD_TERMINATION;
		 }
		 
		 
	 }
	 else
	 {
		 result = RECORD_HEADER_ERROR;
	 }
	 
	 // get page size ==> 16(data in one record) * 8 records = 128 (size of one page)
	 // or get the termination record
	 if((received_Record_Counter == 8) || (result == RECORD_TERMINATION))
	 {
		received_Record_Counter = 0;
		Write_Mem_Page (Mem_Page_To_Write,page_Buffer,page_Buffer_Fill_Level);
		page_Buffer_Fill_Level = 0;
		Mem_Page_To_Write ++;
		 
	 }
	 return result;
 }


 



