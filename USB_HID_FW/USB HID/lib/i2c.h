/*
 * i2c.h
 *
 * Created: 5/28/2016 11:15:14 AM
 *  Author: Nilesh
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "gpio.h"
#define  bool uint8_t

#define SDA_IN PINB
#define SDA_PORT PORTB
#define SCL_PORT PORTB

#define SDA_DDR DDRB
#define SCL_DDR DDRB
#define SDA_PIN 4
#define SCL_PIN 5

#define SDA_INPUT() SDA_DDR &=~(1<<SDA_PIN)
#define SDA_OUTPUT() SDA_DDR |= (1<<SDA_PIN)

#define SCL_INPUT() SCL_DDR &=~(1<<SCL_PIN)
#define SCL_OUTPUT() SCL_DDR |= (1<<SCL_PIN)

#define SDA_LOW() SDA_PORT &=~(1<<SDA_PIN)
#define SDA_HIGH() SDA_PORT |= (1<<SDA_PIN)

#define SCL_LOW() SCL_PORT &=~(1<<SCL_PIN)
#define SCL_HIGH() SCL_PORT |= (1<<SCL_PIN)

#define SDA_READ() ((SDA_IN >> SDA_PIN) & 1)

//#define I2C_CLK_DELAY() delayMicroseconds(I2C_DELAY_USEC)
#define I2C_CLK_DELAY() _delay_us(4)

void i2c_init(void);
bool i2c_write(uint8_t data);
bool i2c_start(uint8_t addressRW);
uint8_t i2c_read(uint8_t last);
bool i2c_restart(uint8_t addressRW);
void i2c_stop(void);

uint8_t i2c_read_buff(uint8_t address, uint8_t *buf, uint8_t count);
uint8_t i2c_write_buff(uint8_t address, uint8_t *buf, uint8_t count);


#endif /* I2C_H_ */