
/*
 Name           : YJVS Srilakshmi
 Batch          : 24008
 */
#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "timer0.h"
#include "ds1307.h"
#include "i2c.h"
#include "eeprom.h"
#include "uart.h"
unsigned char clock_reg[3],time[9],ev[][3]={"ON","GR","GN","G1","G2","G3","G4","G5","-C","VL","DL","CL","ST","CP"};
unsigned int sp,ev_ind=0,main_f=0,menu_f = 0,log_count = 0,password_f = 0,prev_ind = 0,sw_flag=0;


void switch_scroll(char key)
{
    if(key == MK_SW2) //event scroll down
    {
       if(ev_ind > 8)
       {
          ev_ind = prev_ind;
       }
        prev_ind = ev_ind;
        if(sw_flag == 1)
        {
            ev_ind = 2;
            sw_flag = 0;
        }
        else if(ev_ind >=0 && ev_ind <7)
            ev_ind++;
        store_event();
    }
    else if(key == MK_SW3) //event scroll up
    {
        if(ev_ind > 8)
        {
            ev_ind = prev_ind;
        }
        prev_ind = ev_ind;
        if(sw_flag == 1)
        {
            ev_ind = 2;
            sw_flag = 0;
        }
        else if(ev_ind >1 && ev_ind <=7)
           ev_ind--;   
        store_event();
    }
    else if(key == MK_SW1)
    {  
        if(ev_ind > 8)
        {
            ev_ind = prev_ind;
        }
        prev_ind = ev_ind;
        ev_ind = 8;
        sw_flag = 1;
        store_event();
    }
       
        
}

inti_config()
{
    init_clcd();
	init_i2c();
	init_ds1307();
    init_adc();
    init_matrix_keypad();
    init_uart();
    PEIE = 1;
	ADCON1 = 0x0F;
	init_timer0();
	GIE = 1;
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)           //if bit = 0
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);//bit 4
		time[1] = '0' + (clock_reg[0] & 0x0F);//bit0 3
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void main(void) 
{
    inti_config();
    char key;
    get_time(); 
    sp = (read_adc(CHANNEL4))/10.33;
    store_event();  //storing on event
//    write_ext_eeprom(200,'1');
//    write_ext_eeprom(201,'1');
//    write_ext_eeprom(202,'1');
//    write_ext_eeprom(203,'1');
    while (1)
    {
		get_time(); 
        key = read_switches(STATE_CHANGE);
        sp = (read_adc(CHANNEL4))/10.33;
        
        switch_scroll(key);
        if(main_f == DASHBOARD)  //dash board
        {
            dashboard();
            if(key == MK_SW11)
            {
                main_f = 1;
            }
        }
        else if(main_f == MENU)  //menu
        {
            menu(key);
        }
        else if(main_f == PASSWORD) //password
        {
            password(key);
        }
        else if(main_f == MENU_ENTER)  //menu enter
        {
            if(menu_f == VIEWLOG)  //viewlog
            {
                view_log(key);
            }
            else if(menu_f == DOWNLOADLOG)
            {
                download_log();
            }
            else if(menu_f == CLEARLOG)
            {
                clear_log(key);
            }
            else if(menu_f == SETTIME)
            {
                settime(key);
            }
            else if(menu_f == CHANGEPASS)
            {
                change_pass(key);
            }  
        }  
    }
    return;
}
