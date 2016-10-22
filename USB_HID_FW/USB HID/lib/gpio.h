/*
 * gpio.h
 *
 * Created: 6/28/2016 10:12:40 PM
 *  Author: Nilesh Mundphan
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#define DG0 2		//PD2
#define DG1 3		//PD3

#define DG2 1		//PD1
#define DG3 0		//PD0
#define DG4 4		//PD4
#define DG5	30		//PC6 
#define DG6 7		//PD7
#define DG7	38		//PE6
#define DG8 12		//PB4
#define DG9 13		//PB5

#define DG10 14		//PB6
#define DG14 11		//PB3
#define DG15 9		//PB1
#define DG16 10		//PB2

#define A0	23		//PF7
#define A1	22		//PF6
#define A2	21		//PF5 
#define A3	20		//PF4

#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0
#define TOGGLE 2 

#define true 1
#define false 0

void gpio_pin_mode(uint8_t pin_no,uint8_t DD);
void gpio_pin_write(uint8_t pin_no,uint8_t ss);
uint8_t gpio_pin_read(uint8_t pin_no);
uint16_t gpio_anolog_read(uint8_t ch);

#endif /* GPIO_H_ */