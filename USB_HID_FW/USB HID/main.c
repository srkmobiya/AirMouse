/*

 * USB HID.c
 *
 * Created: 6/20/2016 8:46:13 PM
 * Author : Nilesh Mundphan
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "lib/avr_usb.h"
#include "lib/gpio.h"
#include "lib/i2c.h"
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

volatile uint8_t do_output=0;
uint8_t buffer[64];
int counter =0;

int16_t AcX=0,AcY=0,AcZ=0,GcX=0,GcY=0,GcZ=0;

uint8_t str[64]={0};

int main(void)
{
	//int8_t r;
	uint16_t count=0;
	// set for 16 MHz clock
	CPU_PRESCALE(0);
	usb_init();
	while (!usb_configured()) /* wait */ ;
	_delay_ms(1000);
	TCCR0A = 0x00;
	TCCR0B = 0x05;
	TIMSK0 = (1<<TOIE0);
	i2c_init();
	i2c_start(0xD0);
	i2c_stop();
	
	i2c_start(0xD0);
	i2c_write(0x6B);
	i2c_write(0);
	i2c_stop();
	
	while (1) {
		// if received data, do something with it
		//r = usb_rawhid_recv(buffer, 0);
		//r=2;
		//buffer[0]='r';
		//buffer[1]='g';
		/*if (r > 0) 
		{
			if (buffer[0]=='w')
			{
				if (buffer[1]=='m')
				{
					gpio_pin_mode(buffer[2],buffer[3]);
				    usb_rawhid_send((uint8_t *)"Mode is Set", 64);
				}
				else if(buffer[1]=='w')
				{
					gpio_pin_write(buffer[2],buffer[3]);
				    usb_rawhid_send((uint8_t *)"Data Write Done", 64);
				    
				}
			}
			else if (buffer[0]=='r')
			{
				if (buffer[1]=='p')
				{
					gpio_pin_read(buffer[2]);
					usb_rawhid_send((uint8_t *)"Read Done", 64);
					
				}
				else if(buffer[1]=='a')
				{
	
					//This gyro i think
					i2c_read_buff(0x3B,str,14 );
					AcX=str[0]<<8|str[1];
					AcY=str[2]<<8|str[3];
					AcZ=str[4]<<8|str[5];
					sprintf((char* )str,"%d,%d,%d",AcX,AcY,AcZ);
					usb_rawhid_send((uint8_t *)str, 10);
					
				}
				else if(buffer[1]=='g')
				{*/
					
					i2c_read_buff(0x3B,str,14);
					
					GcX=str[8]<<8|str[9];
					GcY=str[10]<<8|str[11];
					GcZ=str[12]<<8|str[13];
					
					sprintf((char* )str,"%d,%d,%d",GcX,GcY,GcZ);
					usb_rawhid_send((uint8_t *)str, 10);
					
					
				//}

			//}
		//}
		// if time to send output, transmit something interesting
		if (do_output) {
			do_output = 0;
					//usb_rawhid_send((uint8_t *)"Test Data", 64);
					
			count++;
		}
	}
}

// This interrupt routine is run approx 61 times per second.
ISR(TIMER0_OVF_vect)
{
	static uint8_t count=0;

	// set the do_output variable every 2 seconds
	if (++count > 122) {
		count = 0;
		do_output = 1;
	}
}
