#include "bsp_ptc.h"
#include "bsp.h"

static void lcd_disp_ptc_value(uint8_t value);

void Ptc_On(void)
{

   PTC_SetHigh();


}

void Ptc_Off(void)
{

   PTC_SetLow();

}


void set_ptc_value_conifrm_handler(uint8_t value)
{

   lcd_donot_temp_value_fun();
   osDelay(500);

   lcd_disp_ptc_value(value);
}



static void lcd_disp_ptc_value(uint8_t value)
{
     
   glcd_t.number1_low =  value   / 10 ;
   glcd_t.number1_high = value   / 10 ;
   glcd_t.number2_low  =  value   % 10; //
   glcd_t.number2_high =  value   % 10; //
  LCD_Disp_Temperature_Value_Handler();
}



