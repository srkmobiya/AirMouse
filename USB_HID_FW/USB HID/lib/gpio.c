/*
 * gpio.c
 *
 * Created: 6/28/2016 10:12:27 PM
 *  Author: Nilesh Mundphan
 */ 

 #include <avr/io.h>
 #include "gpio.h"

 void gpio_pin_mode(uint8_t pin_no,uint8_t DD)
 {
	 if (pin_no<8)
	 {
		 if (DD==INPUT)
		 {
			 DDRD &=~(1<<pin_no);
		 }
		 else
		 {
			 DDRD|=(1<<pin_no);
		 }
	 }
	 else if (pin_no<16)
	 {
		 pin_no-=8;
		 if (DD==INPUT)
		 {
			 DDRB &=~(1<<pin_no);
		 }
		 else
		 {
			 DDRB|=(1<<pin_no);
		 }
	 }
	 else if (pin_no<24)
	 {
		 pin_no-=16;
		 if (DD==INPUT)
		 {
			 DDRF &=~(1<<pin_no);
		 }
		 else
		 {
			 DDRF|=(1<<pin_no);
		 }
	 }
	 else if (pin_no<32)
	 {
	     pin_no-=24;
	     if (DD==INPUT)
	     {
		     DDRC &=~(1<<pin_no);
	     }
	     else
	     {
		     DDRC|=(1<<pin_no);
	     }    
	 }
	 else if (pin_no<40)
	 {
		 pin_no-=32;
		 if (DD==INPUT)
		 {
			 DDRE &=~(1<<pin_no);
		 }
		 else
		 {
			 DDRE|=(1<<pin_no);
		 }
	 }
 }

 void gpio_pin_write(uint8_t pin_no,uint8_t ss)
 {
	 if (pin_no<8)
	 {
		 if (ss==LOW)
		 {
			 PORTD &=~(1<<pin_no);
		 }
		 else if(ss==HIGH)
		 {
			 PORTD|=(1<<pin_no);
		 }
		 else 
		 {
			 PORTD^=(1<<pin_no);
		 }

	 }
	 else if (pin_no<16)
	 {
		 pin_no-=8;
		 if (ss==LOW)
		 {
			 PORTB &=~(1<<pin_no);
		 }
		 else if(ss==HIGH)
		 {
			 PORTB|=(1<<pin_no);
		 }
		 else
		 {
			 PORTB^=(1<<pin_no);
		 }
	 }
	 else if (pin_no<24)
	 {
		 pin_no-=16;
		 if (ss==LOW)
		 {
			 PORTF &=~(1<<pin_no);
		 }
		 else if(ss==HIGH)
		 {
			 PORTF|=(1<<pin_no);
		 }
		 else
		 {
			 PORTF^=(1<<pin_no);
		 }
	 }
	 else if (pin_no<32)
	 {
		 pin_no-=24;
		 if (ss==LOW)
		 {
			 PORTC &=~(1<<pin_no);
		 }
		 else if(ss==HIGH)
		 {
			 PORTC|=(1<<pin_no);
		 }
		 else
		 {
			 PORTC^=(1<<pin_no);
		 }
	 }
	 else if (pin_no<40)
	 {
		 pin_no-=32;
		 if (ss==LOW)
		 {
			 PORTE &=~(1<<pin_no);
		 }
		 else if(ss==HIGH)
		 {
			 PORTE|=(1<<pin_no);
		 }
		 else
		 {
			 PORTE^=(1<<pin_no);
		 }
	 }
 }

 uint8_t gpio_pin_read(uint8_t pin_no)
 {
	 if (pin_no<8)
	 {
		 return ((1<<pin_no) & PIND);
	 }
	 else if (pin_no<15)
	 {
		 pin_no-=8;
		 return ((1<<pin_no) & PINB);
	 }
	 else if (pin_no<24)
	 {
		 pin_no-=16;
		 return ((1<<pin_no) & PINF);
	 }
	 else if (pin_no<32)
	 {
		 pin_no-=24;
		 return ((1<<pin_no) & PINC);
	 }
	 else if (pin_no<40)
	 {
		 pin_no-=32;
		 return ((1<<pin_no) & PINE);
	 }
	 return 1;
 }

 uint16_t gpio_anolog_read(uint8_t ch)
 {
	 ADMUX|=(1<<REFS0);
	 ADCSRA=(1<<ADEN)|(7<<ADPS0);
	 ch=ch&0b00000111;
	 ADMUX&=0b11100000;
	 ADMUX|=ch;
	 ADCSRA|=(1<<ADSC);
	 while(!(ADCSRA & (1<<ADIF)));
	 ADCSRA|=(1<<ADIF);
	 return(ADC);
 }

