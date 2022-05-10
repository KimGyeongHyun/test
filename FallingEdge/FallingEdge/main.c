/*
 * FallingEdge.c
 *
 * Created: 2022-05-10 오후 3:08:18
 * Author : poor1
 */ 
#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "millis.h"

void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);//set baud rate
	UBRR0L = (unsigned char)ubrr;//set baud rate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);//receive and transmitter enable
	UCSR0C = (3 << UCSZ0);//8-bit character size
}

//function to write character to output
void USART_TX(unsigned char data)
{
	while (!(UCSR0A & (1 << UDRE0)));//if the data register is full, wait
	UDR0 = data;//write data to UDR0 register
}

//function to output string
void USART_TX_String(const char *text)
{
	while (*text != 0)//until the end of the string is reached
	USART_TX(*text++);//print each character in a string
}





bool isHigh;

bool temp_Falling_Edge(){
	if ((PIND & 0x01) == 0x01)
		isHigh = true;
	else
		if (isHigh == true)
		{
			isHigh = false;
			PORTF = PORTF | 0x01;
			_delay_ms(1000);
			PORTF = PORTF & 0x00;
			return true;
		}
	return 0;
}

bool D0high = false, D1high = false;

bool D0_Falling_Ege(){
	if ((PIND & 0x01) == 0x01)
		D0high = true;
	else
		if (D0high == true)
		{
			D0high = false;
			return true;
		}
	return false;
}

bool D1_Falling_Ege(){
	if ((PIND & 0x02) == 0x02)
	D1high = true;
	else
	if (D1high == true)
	{
		D1high = false;
		return true;
	}
	return false;
}




int main(void)
{
	USART_Init(MYUBRR);
	init_millis(F_CPU);
	
	DDRD = 0x00;
	DDRF = 0xFF;
	int countD0 = 0, countD1 = 0;
	char bufferD0[10], bufferD1[10];
	
    /* Replace with your application code */
    while (1) 
    {
		//temp_Falling_Edge();
		if(D0_Falling_Ege() == true)	countD0++;
		if(D1_Falling_Ege() == true)	countD1++;
		itoa(countD0, bufferD0, 10);
		itoa(countD1, bufferD1, 10);
		
		
		if(millis() % 1000 == 0.0)
		{
			USART_TX_String("D0 : ");
			USART_TX_String(bufferD0);
			USART_TX_String("\r\n");
			USART_TX_String("D1 : ");
			USART_TX_String(bufferD1);
			USART_TX_String("\r\n");
			USART_TX_String("\r\n");
		}
    }
}

