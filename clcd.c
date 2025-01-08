
#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char byte, unsigned char control_bit)
{
	CLCD_RS = control_bit; //assign the rs bit value 0-instr, 1-data
	CLCD_PORT = byte;       // assigning instr/data to the data port

	/* Should be atleast 200ns */
	CLCD_EN = HI;           //1 clock pulse to get the value assigned
	CLCD_EN = LO;

	PORT_DIR = INPUT;       //direction of RD7 as i/p
	CLCD_RW = HI;           //for reading making rw bit as 1
	CLCD_RS = INSTRUCTION_COMMAND;    //reading instr from clcd

	do
	{
		CLCD_EN = HI;
		CLCD_EN = LO;
	} while (CLCD_BUSY);        //check for D7,D7=0 completed
                                            // D7=1 not completed
	CLCD_RW = LO;           //rw = 0 cuz next we are writing 
	PORT_DIR = OUTPUT;      //making rw direction as output
}

void init_clcd()
{
	/* Set PortD as output port for CLCD data */
	TRISD = 0x00;
	/* Set PortC as output port for CLCD control */
	TRISC = TRISC & 0xF8;

	CLCD_RW = LO;

	
     /* Startup Time for the CLCD controller */
    __delay_ms(30);
    
    /* The CLCD Startup Sequence */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(4100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(100);
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND	);
    __delay_us(1);   //1sec delay 
    
    CURSOR_HOME;
    __delay_us(100);
    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);
    CLEAR_DISP_SCREEN;
    __delay_us(500);
    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

void clcd_print(const unsigned char *data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	while (*data != '\0')
	{
		clcd_write(*data++, DATA_COMMAND);
	}
}

void clcd_putch(const unsigned char data, unsigned char addr)
{
	clcd_write(addr, INSTRUCTION_COMMAND);
	clcd_write(data, DATA_COMMAND);
}
