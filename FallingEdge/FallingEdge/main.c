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
#include <avr/interrupt.h>

#define PEOPLE_COUNT_PIN INT2
#define CAR_COUNT_PIN INT3

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





// count car and people
Counter_Init()
{
	// falling edge
	// use INT2, INT3
	EICRA &= ~(1 << ISC20);
	EICRA = 1 << ISC21;
	EICRA &= ~(1 << ISC30);
	EICRA = 1 << ISC31;
	
	// interrupt enable
	EIMSK |= (1 << PEOPLE_COUNT_PIN) | (1 << CAR_COUNT_PIN);
}

int peopleCount = 0, carCount = 0;

// count people
ISR(INT2_vect){
	peopleCount++;
}

// count car
ISR(INT3_vect){
	carCount++;
}


// 사람과 자동차 count 보여주는 함수
void Print_Falling_Edge(){
	USART_TX_String("People count : ");
	itoa(peopleCount, buffer, 10);
	USART_TX_String(buffer);
	USART_TX_String("\r\n");
	USART_TX_String("Car count : ");
	itoa(carCount, buffer, 10);
	USART_TX_String(buffer);
	USART_TX_String("\r\n");
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
		Print_Falling_Edge();
    }
}

