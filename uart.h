#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);  //transmitting 1 byte
int puts(const char *s);   //transmitting a string
unsigned char getch(void);  //receiving 1 byte
unsigned char getch_with_timeout(unsigned short max_time);  
unsigned char getche(void);     //transmitting, receiving 1 byte 

#endif
