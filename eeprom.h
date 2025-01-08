
#ifndef EXT_eeprom
#define EXT_eeprom

#define SLAVE_READ_eeprom		0xA1
#define SLAVE_WRITE_eeprom		0xA0


void write_ext_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_ext_eeprom(unsigned char address1);

#endif