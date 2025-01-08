#include <xc.h>
#include "main.h"
#include "i2c.h"
#include "eeprom.h"



/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */


void write_ext_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int i=3000;i--;);
}

unsigned char read_ext_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_eeprom);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_eeprom);
	data = i2c_read();
	i2c_stop();
    for(int i=3000;i--;);
	return data;
}