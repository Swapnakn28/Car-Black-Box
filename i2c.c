#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode */
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;
	/* Clear bus collision flag */
	BCLIF = 0;
}
//to check the instruction is executed or not
void i2c_idle(void)
{
	while (!SSPIF);
    //0-instruction is not completed 
	SSPIF = 0;
}

void i2c_ack(void)
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}
//to start the communication
void i2c_start(void)
{
	SEN = 1;//SSPCON2
	i2c_idle();//to check the communication is started or not
}
//to stop communication
void i2c_stop(void)
{
	PEN = 1;//SSPCON2
	i2c_idle();//to check the communication is stopped or not
}
//to initiate the repeat start 
void i2c_rep_start(void)
{
	RSEN = 1;
	i2c_idle();//to check ,repeat start is initiiated or not
}
//to transmit 1 byte of data
void i2c_write(unsigned char data)
{
	SSPBUF = data;
	i2c_idle();//to check,data is transmitted or not
}

void i2c_rx_mode(void)
{
	RCEN = 1;
	i2c_idle();
}
//to send "no acknowledgment" to RTC 
void i2c_no_ack(void)
{
	ACKDT = 1;
	ACKEN = 1;
}
//to receive 1 byte of data
unsigned char i2c_read(void)
{
	i2c_rx_mode();//to enable the receive mode
	i2c_no_ack();//to send the NOT  acknowledgment to RTC 

	return SSPBUF;
}