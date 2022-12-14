#define F_CPU 16000000UL //needs to be defined for the delay functions to work.
#define BAUD 9600
#define NUMBER_STRING 1001
#include <avr/io.h> //used for pins input/output
#include <util/delay.h> //here the delay functions are found
#include <stdio.h> //used for printf function.
#include "usart.h"//for uart output to pc. Debugging purposes
#include <avr/interrupt.h>
#define PI (3.141592653589793)


double dutyCycle = 0;
int TIME, PPR; 
double coef;
float voltage;
float carspeed;
int distancetravelled;
unsigned int first_dist= 0;
unsigned int sec_dist = 0;
unsigned int first_time = 0;
unsigned int sec_time = 0;



void PWM_speed_1()
{
    dutyCycle = (first_dist/first_time)*coef;  
}
void PWM_speed_2()
{
    dutyCycle = (sec_dist/sec_time)*coef;
}
unsigned int nextion_value_for_first_distance(void);
unsigned int nextion_value_for_second_distance(void);
unsigned int nextion_value_for_first_time(void);
unsigned int nextion_value_for_second_time(void);

int main(void)
{   
    uart_init(); 
	io_redirect();
    DDRB = 0x00; //output
    PORTB = 0x01; //pin 1-i
    //Setup timer/counter as pure ticks counter, counting //clock ticks with 1024 prescaling ir. no interrupts
    TCCR1A = 0X00;
    TCCR1B = 0xC5;
    //Input capturee on positive edge ICP1 (PBO)
    //Filter is enabled 1024 clock prescaler
    DDRB &= ~0x01;//pin1 as input for ICP1 use 
    PORTB |= 0x01; //enable pullups
    TCNT1=0;
    //check ICR1 (which is a register) page.110
    //Take a look at registers in big file 
    int count = 0;
    DDRD = (1 << PORTD6);
    
    TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01);
    TIMSK0 = (1 << TOIE0);
    
    OCR0A = (dutyCycle/100.0)*255.0;

    sei();
    
    TCCR0B = (1 << CS00) | (1 << CS02);
    char readBuffer[100];
    printf("page 0%c%c%c",255,255,255);//init at 9600 baud.
	_delay_ms(20);
	
	int i;
    if(voltage==1)
	{
		while (1) 
		{
      
			scanf("%c", readBuffer[i]);
			for(int i = 0 ; i<7 ; i++)
			{
				scanf("%c", &readBuffer[i]);
				if (readBuffer[0] == 0x65 && readBuffer[2] == 0x0F && readBuffer[4] == 0xFF && readBuffer[5] == 0xFF && readBuffer[6] == 0xFF) 
				{
					// beginning of the process for first distance
					first_dist = nextion_value_for_first_distance();
					// end of the process for first distance

					// beginning of the process for second distance
					sec_dist = nextion_value_for_second_distance();
					// end of process for second distance

					// beginning of the process for first time value
					first_time = nextion_value_for_first_time();
					// end of the process for the first time value

					//beginning of the process for the second time value
					sec_time = nextion_value_for_second_time();
					// end of the process for the second time value
				}
				else
				{	
						while( (TIFR1 & 1<<5) == (1<<5))
						{
							//Make sure 5th slot is recording data

							int temptime = 0; // delete if acceleration code does not work
							
							TIME = ICR1 * (64.0/100000); //Time diff between holes -> time is recorded
							PPR = TIME * 40; //number of holes in encoder wheel needs to be doubled
							
							//delete from here if acceleration code does not work
							if(TIME > temptime)
							{
								//braking = true;
								printf("page play.t6.txt=%c%c%c%c", 'N',255,255,255);
								printf("page play.t7.txt=%c%c%c%c", 'Y',255,255,255);
							}
							if(TIME < temptime)
							{
								printf("page play.t6.txt=%c%c%c%c", 'Y',255,255,255);
								printf("page play.t7.txt=%c%c%c%c", 'N',255,255,255);
							}
							temptime = TIME;
							//delete till here if acceleration code does not work

							//trash from here if optocoupler progress bar does not work with distancetravelled
						// int onerotation = 1.570796326795;
							float distance = 64.9*PI; // wheels circumference = diameter * PI
							count = count++;
							distancetravelled = distance * count; 
							//trash till here if optocoupler progress bar does not work with distancetravelled

							float carspeed = distance/PPR;

							// this is more complicated 
							// Ang_V = 2*PI/PPR;
							// velocity = Ang_V*11.5; //diameter of wheel * pi / rotational period;
							
							TCNT1 = 0;
							TIFR1 |= (1<<5);

							TCNT1 = 0;
							TIFR1 |=(1<<5);
						}
						PWM_speed1();
						//10-15 diameter
					// printf("The speed of the car is %.2f m/s", carspeed);
						if(distancetravelled == (int)first_dist )
						{
							PWM_speed_2();
						}
						if(distancetravelled == (int)first_dist + (int)sec_dist)
						{
							dutyCycle=0;
						}

				}
			
        
    		}
		}  
	}	
	else
	{
		
		printf("page warning%c%c%c",255,255,255);//init at 9600 baud. if the voltage of the battery is bellow 6.8 diplay shows warning page
		dutyCycle = 0;
		
	}
}




//int main
 
ISR(TIMER0_OVF_vect)
{
    OCR0A = (dutyCycle/100.0)*255;
}
unsigned int nextion_value_for_first_distance(void)
{
	uint32_t first_distance_value;
	printf("get page1.n0.val%c%c%c",255,255,255);	//sends "get page1.n0.val"	
	for(int i = 0; i<8;i++)
	{
		scanf("%c", &readBuffer[i]);
		if(readBuffer[i] == 0x71)//Expect number string
		{
			readBuffer[0] = 0x71;//Move indicator to front, just to keep the nice format
			break;
		}
	}
	if(readBuffer[0] == 0x71) 
	{
		for(int i = 1; i<8; i++)
		{
			scanf("%c", &readBuffer[i]);
		}

		if(readBuffer[0] ==(char)0x71 && readBuffer[5] == (char)0xFF && readBuffer[6] ==(char) 0xFF && readBuffer[7] == (char)0xFF)//This is a complete number return
		{
			first_distance_value = readBuffer[1] | (readBuffer[2] << 8) | (readBuffer[3] << 16)| (readBuffer[4] << 24);
		}
	}
	return((unsigned int)first_distance_value);
}
unsigned int nextion_value_for_second_distance(void)
{
	unsigned int second_distance_value;
	printf("get page1.n1.val%c%c%c",255,255,255);	//sends "get page1.n1.val"	
						
	for(int i = 0; i<8;i++)
	{
		scanf("%c", &readBuffer[i]);
		if(readBuffer[i] == 0x71)//Expect number string
		{
			readBuffer[0] = 0x71;//Move indicator to front, just to keep the nice format
			break;
		}
	}
	if(readBuffer[0] = 0x71)
	{
		for(int i = 1; i<8; i++)
		{
			scanf("%c", &readBuffer[i]);
		}

		if(readBuffer[0] ==(char)0x71 && readBuffer[5] == (char)0xFF && readBuffer[6] ==(char) 0xFF && readBuffer[7] == (char)0xFF)//This is a complete number return
		{
			sec_dist = readBuffer[1] | (readBuffer[2] << 8)| (readBuffer[3] << 16)| (readBuffer[4] << 24);
		}
	}	
	return((unsigned int)second_distance_value);
}
unsigned int nextion_value_for_first_time(void)
{	
	unsigned int first_time_value;
	printf("get page1.n3.val%c%c%c",255,255,255);	//sends "get page1.n3.val"
					
	for(int i = 0; i<8;i++)
	{
		scanf("%c", &readBuffer[i]);
		if(readBuffer[i] == 0x71)//Expect number string
		{
			readBuffer[0] = 0x71;//Move indicator to front, just to keep the nice format
			break;
		}
	}
	if(readBuffer[0] == 0x71)
	{
		for(int i = 1; i<8; i++)
		{
			scanf("%c", &readBuffer[i]);
		}

		if(readBuffer[0] ==(char)0x71 && readBuffer[5] == (char)0xFF && readBuffer[6] ==(char) 0xFF && readBuffer[7] == (char)0xFF)//This is a complete number return
		{
			first_time_value = readBuffer[1] | (readBuffer[2] << 8) | (readBuffer[3] << 16)| (readBuffer[4] << 24);
		}
	}
	return((unsigned int) first_time_value);
}
unsigned int nextion_value_for_second_time(void)
{
		uint32_t second_time_value;
		printf("get page1.n4.val%c%c%c",255,255,255);	//sends "get page0.n0.val"	
					
		for(int i = 0; i<8;i++)
		{
			scanf("%c", &readBuffer[i]);
			if(readBuffer[i] == 0x71)//Expect number string
			{
				readBuffer[0] = 0x71;//Move indicator to front, just to keep the nice format
				break;
			}
		}
		if(readBuffer[0] == 0x71)
		{
			for(int i = 1; i<8; i++)
			{
				scanf("%c", &readBuffer[i]);
			}

			if(readBuffer[0] ==(char)0x71 && readBuffer[5] == (char)0xFF && readBuffer[6] ==(char) 0xFF && readBuffer[7] == (char)0xFF)//This is a complete number return
			{
				second_time_value = readBuffer[1] | (readBuffer[2] << 8) | (readBuffer[3] << 16)| (readBuffer[4] << 24);
			}
		}
		return((unsigned int)second_time_value);
}