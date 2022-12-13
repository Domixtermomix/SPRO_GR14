#define F_CPU 16000000UL //needs to be defined for the delay functions to work.
#define BAUD 9600
#define NUMBER_STRING 1001

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h> //here the delay functions are found
#include "usart.h"

int main(void)
{
	char readBuffer[100];
	uart_init();//initialize communication with PC - debugging
	io_redirect();//redirect printf function to uart, so text will be shown on PC
	
	printf("page 0%c%c%c",255,255,255);//init at 9600 baud.
	_delay_ms(20);
	uint32_t readValue = 0;
    while (1) 
    {
		
		int readValue;
		scanf("%c", readBuffer[i]);
		for(int i = 0 ; i<7 ; i++)
		{
			scanf("%c", readBuffer[i]);
			if(readBuffer[0] ==0x65 && readBuffer[2] == 0x07 && readBuffer[4] == 0xFF && readBuffer[5] == 0xFF && readBuffer[6] == 0xFF )
			{
				printf("get page0.n0.val%c%c%c",0xFF,0xFF,0xFF); //getting the value of the first distance
				break;
			}
			else
			{
				printf("get pag0.n1.val%c%c%c",0xFF,0xFF,0xFF); // getting the value of the first time
				break;
			}
			break;
		}
	
	// start button
		for( int i = 0; i < 7; i++)
		{
			scanf("%c" &readBuffer[i]);
			if(readBuffer[0] == 0x65 && readBuffer[1] == 0x02 && readBuffer[2] == 0x01 && readBuffer[4] == 0xFF && readBuffer[5] == 0xFF && readBuffer[6] == 0xFF)
			{
				// run engine code + 
				/*unsigned int batteryvalue;
				if ((float)batteryvalue < 6.8)
				{
					printf("page warning%c%c%c",255,255,255);//init at 9600 baud. if the voltage of the battery is bellow 6.8 diplay shows warning page
				}
				else
				{
					printf("play.x0.val=%d%c%c%c",(float)batteryvalue , 0xFF, 0xFF, 0xFF);// if the voltage is above that number in the play page the voltage will be visible 
				}*/
			}
		}	
	}

} 
	 

   /* while (1) 
    {
		int	typeExpected = 0;
		for(int i = 1; i<7; i++)
		{
			scanf("%c", &readBuffer[i]);
			if(readBuffer[i]== 0x65)
			{
				typeExpected = BUTTON_STRING;
				readBuffer[0] = 0x65;
				break;
			}
		}
		if (typeExpected == BUTTON_STRING)
		{
			for (int i = 1; i < 7; i++)
			{
				scanf("%c", &readBuffer[i]);
			}
			if(readBuffer[0] == 0x65 && readBuffer[2] == 0x03 && readBuffer[4] == 0xFF && readBuffer[5] == 0xFF && readBuffer[6] == 0xFF)
			{
					readValue = 20;
			}	
		}
			for(int i = 0; i<7; i++)
		{
				scanf("%c", &readBuffer[i]);
				if(readBuffer[i] == 0x1A)//some error occurred - retrieve the 0xFF commands and start over
				{
					scanf("%c", &readBuffer[i]);
					scanf("%c", &readBuffer[i]);
					scanf("%c", &readBuffer[i]);
					continue;
				}
		}
		
		printf("page0.distance1.val=%d%c%c%c", (int)readValue, 255,255,255);
		_delay_ms(100);
	}
}

	*/
