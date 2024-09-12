#include "bsp_fan.h"
#include "bsp.h"

uint8_t fan_full_speed,fan_lower_speed,fan_middle_speed,fan_stop_flag;
void FAN_GPIO_Iint(void)
{

   GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();


  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = FAN_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void Fan_Run(void)
{
   FAN_COM_SetLow(); //PA6
   MX_TIM16_Init(10); //100KHZ,duty =50%

   HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);//ultrasnoic On
   
	 

}

void fan_max_run(void)
{
   FAN_COM_SetLow(); //PA6
   MX_TIM16_Init(10); //100KHZ,duty =50%
   HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);//ultrasnoic On
  
}




void Fan_Run_Middle(void)
{



   FAN_COM_SetLow(); //PA6
   MX_TIM16_Init(9); //100KHZ,duty =50%
   HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);//ultrasnoic On
 


}

void Fan_Run_Lower(void)
{

  MX_TIM16_Init(8); //100KHZ,duty =50%

   
   
   HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);//ultrasnoic On
   FAN_COM_SetLow(); //PA6


}

 
void Fan_Stop(void)
{
  

    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);//ultrasnoic On

    FAN_COM_SetLow(); //PA6
  
}




void fan_run_state_handler(void)
{
   
       if(gkey_t.key_mode  != mode_set_timer){
           LCD_Wind_Run_Icon(wifi_t.set_wind_speed_value);

             

      }
      

}


