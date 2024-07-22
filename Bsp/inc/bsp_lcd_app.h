#ifndef __BSP_LCD_APP_H
#define __BSP_LCD_APP_H
#include "main.h"



void PowerOn_Init(void);


void LCD_Disp_Works_Timing_Init(void);

void LCD_Disp_Timer_Timing_Init(void);


void MainBoard_Run_Feature_Handler(void);



void PowerOff_freeFun(void);

void PowerOff_Off_Led(void);


void Display_MainBoard_Feature_Handler(void);

void Display_Works_Timing(void);


void Display_Timer_Timing(void);

//void LCD_Disp_Timer_Timing(void);


void Display_WorksTimingr_Handler(uint8_t sel_item);


void Set_Timer_Timing_Lcd_Blink(void);



//void Record_WorksTime_DonotDisp_Handler(void);


void Mainboard_Action_Fun(void);

void Set_LCD_Disp_Timer_Value(void);

void LCD_Disp_Set_Timer_Timing(void);


void Record_WorksOr_Timer_Timing_DonotDisp_Handler(void);




//void Set_Timer_Timing_Lcd_Blink_Off(void);
//void Set_Timer_Timing_Lcd_Blink_On(void);

#endif


