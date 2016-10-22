/*
 * i2c.c
 *
 * Created: 5/28/2016 11:15:00 AM
 *  Author: Nilesh Mundphan
 */ 
 
 #include <avr/io.h>
 #include <util/delay.h>
 #include "i2c.h"
 
 uint8_t const I2C_READ = 1;
 uint8_t const I2C_WRITE = 0;
 
 #define  SLAVE_ADDR 0xD0

 void i2c_init(void) {
	 SDA_OUTPUT();
	 SDA_HIGH();
	 SCL_OUTPUT();
	 SCL_HIGH();
 }

 bool i2c_write(uint8_t data) {
	 for (uint8_t m = 0X80; m != 0; m >>= 1)
	 {
		 if((m & data) == LOW)
		 {
			 SDA_LOW();
		 }
		 else
		 {
			 SDA_HIGH();
		 }
		 
		 SCL_HIGH();
		 I2C_CLK_DELAY();
		 SCL_LOW();
		 I2C_CLK_DELAY();
	 }
	 
	 SDA_INPUT();
	 // get Ack or Nak
	 // enable pullup
	 SDA_HIGH();
	 SCL_HIGH();
	 I2C_CLK_DELAY();
	 uint8_t rtn = SDA_READ();
	 SCL_LOW();
	 SDA_OUTPUT();
	 SDA_LOW();
	 return rtn == 0;
 }

 bool i2c_start(uint8_t addressRW) {
	 SDA_LOW();
	 I2C_CLK_DELAY();
	 SCL_LOW();
	 return i2c_write(addressRW);
 }
 
 uint8_t i2c_read(uint8_t last) {
	 uint8_t b = 0;
	 SDA_HIGH();
	 SDA_INPUT();
	 for (uint8_t i = 0; i < 8; i++) {
		 b <<= 1;
		 I2C_CLK_DELAY();
		 SCL_HIGH();
		 if (SDA_READ()) b |= 1;
		 SCL_LOW();
	 }

	 // send Ack or Nak
	 SDA_OUTPUT();
	 if((last) == LOW)
	 {
		 SDA_LOW();
	 }
	 else
	 {
		 SDA_HIGH();
	 }
	 SCL_HIGH();
	 I2C_CLK_DELAY();
	 SCL_LOW();
	 SDA_LOW();
	 return b;
 }

 bool i2c_restart(uint8_t addressRW) {

	 SDA_HIGH();
	 SCL_HIGH();
	 I2C_CLK_DELAY();

	 return i2c_start(addressRW);
 }

 void i2c_stop(void) {
	 SDA_LOW();
	 I2C_CLK_DELAY();
	 SCL_HIGH();
	 I2C_CLK_DELAY();
	 SDA_HIGH();
	 I2C_CLK_DELAY();
 }

 uint8_t i2c_read_buff(uint8_t address, uint8_t *buf, uint8_t count) {
	 if (!i2c_start(SLAVE_ADDR | I2C_WRITE)) return false;
	 if (!i2c_write(address)) return false;
	 if (!i2c_restart(SLAVE_ADDR | I2C_READ))return false;
	 for (uint8_t i = 0; i < count; i++) {
		 buf[i] = i2c_read(i == (count-1));
		 _delay_ms(10);
	 }
	 i2c_stop();
	 return true;
 }

 uint8_t i2c_write_buff(uint8_t address, uint8_t *buf, uint8_t count) {
	 if (!i2c_start(SLAVE_ADDR | I2C_WRITE)) return false;
	 if (!i2c_write(address)) return false;
	 for (uint8_t i = 0; i < count; i++) {
		 if (!i2c_write(buf[i])) return false;
		 _delay_ms(10);
	 }
	 i2c_stop();
	 return true;
 }
