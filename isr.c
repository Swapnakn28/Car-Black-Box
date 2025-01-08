
#include <xc.h>
#include "main.h"

unsigned int c=120;
void __interrupt() isr(void)
{
	static unsigned short count;
    
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count++ == 20000)
		{
			count = 0;
			c--;
		}
		TMR0IF = 0;
	}
}