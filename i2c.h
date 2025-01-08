#ifndef I2C_H
#define I2C_H


void init_i2c(void);//configuration to i2c
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);//used to stop communication
void i2c_write(unsigned char data);
unsigned char i2c_read(void);//read data from slave

#endif