#include "bsp_key.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


key_fun_t gkey_t;

/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void power_long_key_fun(void)
{

  
    gkey_t.power_key_long_counter++;
       
         

          
             	//WIFI CONNCETOR process
			 gkey_t.wifi_led_fast_blink_flag=1;
          
           
			 //WIFI CONNCETOR process
			wifi_t.esp8266_login_cloud_success =0;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
            wifi_t.link_tencent_step_counter=0;
           
			wifi_t.power_on_login_tencent_cloud_flag=0;
			

            
			wifi_t.gTimer_linking_tencent_duration=0; //122 -2分7秒
			gpro_t.gTimer_get_data_from_tencent_data=0;
         
          

      

    
}
   
void power_on_key_handler(void)
{
        gkey_t.power_key_long_counter=0;

      
           if(gkey_t.key_power==power_off){
              gkey_t.key_power=power_on;
              gkey_t.key_mode = disp_timer_timing;
               gctl_t.ai_flag = 1;
               gctl_t.ptc_warning =0;
               gctl_t.fan_warning =0;
              
              gctl_t.step_process=0;
              gpro_t.power_off_flag =1;
            }
           else{
              gkey_t.key_power=power_off;
              gctl_t.step_process=0;

           }
          
}
     /*********************************************************************************
*
*	函 数 名:void smartphone_power_on_handler(uint8_t phone_on)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void smartphone_power_on_handler(void)
{

      
      gkey_t.power_key_long_counter=0;

      gkey_t.key_power=power_on;
      gkey_t.key_mode = disp_timer_timing;
       gctl_t.ai_flag = 1;
       gctl_t.ptc_warning =0;
       gctl_t.fan_warning =0;
      
        gctl_t.step_process=0;
        gpro_t.power_off_flag =1;
            
          
    //   Buzzer_KeySound();

      
       
      
}

/*********************************************************************************
*
*	函 数 名:static void mode_long_short_key_fun(void)
*	功能说明: 
*	形    参: 
*	返 回 值: 无
*   
*********************************************************************************/
void mode_long_key_fun(void)
{
    


        gkey_t.key_mode_long_counter++;
  
            gkey_t.key_mode_long_counter = 150;

            gkey_t.key_mode = mode_set_timer;
           gkey_t.key_add_dec_mode = mode_set_timer;
           gctl_t.ai_flag = 0; //timer tiiming model
           gkey_t.gTimer_disp_set_timer = 0;       //counter exit timing this "mode_set_timer"
          
   
           gpro_t.gTimer_timer_led_blink =0;
           

        

    
}


void mode_key_fun(void)
{
  

        gkey_t.key_mode_long_counter=0;

     
         if(gkey_t.key_mode  == disp_works_timing){
             gkey_t.key_mode  = disp_timer_timing;
           
               gctl_t.ai_flag = 0; // DON'T DISP AI ICON
               //counter exit timing this "mode_set_timer"
            gkey_t.key_mode_switch_flag = 1;
            gkey_t.key_add_dec_mode = set_temp_value_item;
            
             LCD_Disp_Timer_Timing_Init();
             disp_ai_iocn();
           
          
            // HAL_Delay(10);
             
            
            
            gkey_t.key_mode_be_pressed = 2;

           

        }
        else{
            gkey_t.key_mode_switch_flag = 1;
            gkey_t.key_mode  = disp_works_timing;
            gkey_t.key_add_dec_mode = set_temp_value_item;
            gctl_t.ai_flag = 1; // AI DISPLAY AI ICON
           
            LCD_Disp_Works_Timing_Init();
            disp_ai_iocn();
            gkey_t.key_mode_be_pressed = 1;
             
        }


}

 


void  key_mode_be_pressed_send_data_wifi(void)
{
   
   if(gkey_t.key_mode_be_pressed == 1 && wifi_link_net_state()==1){

         gkey_t.key_mode_be_pressed= 0xff;
    
        MqttData_Publish_SetState(1); //timer model  = 2, works model = 1
        osDelay(20);
     }
     else if(gkey_t.key_mode_be_pressed == 2  && wifi_link_net_state()==1){
            gkey_t.key_mode_be_pressed= 0xff;

          MqttData_Publish_SetState(2); //timer model  = 2, works model = 1
           osDelay(20);
       }


}


/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Dec_Key_Fun(uint8_t cmd)
{

 

    switch(cmd){

         case set_temp_value_item: //set temperature 

             gpro_t.gTimer_run_main_fun =0;
             gpro_t.gTimer_run_dht11=0; //不显示，实际的温度值，显示设置的温度
             gctl_t.gTimer_compare_ptc_value=0;
         
             gctl_t.gSet_temperature_value  --;
			if( gctl_t.gSet_temperature_value  <20)  gctl_t.gSet_temperature_value  =40;
	        else if( gctl_t.gSet_temperature_value   >40) gctl_t.gSet_temperature_value  =40;

           if( gctl_t.gSet_temperature_value   > 40) gctl_t.gSet_temperature_value  = 20;
				
			glcd_t.number1_low =  gctl_t.gSet_temperature_value   / 10 ;
            glcd_t.number1_high =  gctl_t.gSet_temperature_value   / 10 ;
           
			glcd_t.number2_low  = gctl_t.gSet_temperature_value   % 10; //
            glcd_t.number2_high =  gctl_t.gSet_temperature_value   % 10; //

        
          gkey_t.set_temp_value_be_pressed =1; //flag 
           gctl_t.gTimer_set_temp_value =0;

         
           gctl_t.smart_phone_manual_on_off=0; //unfreeze maybe turn on PTC heat 
      
         //   LCD_Disp_Temperature_Value_Handler();
         break;

         case mode_set_timer: //set timer timing value 


              gkey_t.gTimer_disp_set_timer = 0; 

              gpro_t.set_timer_timing_minutes =0;

              gpro_t.set_timer_timing_hours -- ;//run_t.dispTime_minutes = run_t.dispTime_minutes - 1;
				if(gpro_t.set_timer_timing_hours < 0){//if(run_t.dispTime_minutes < 0){

				    gpro_t.set_timer_timing_hours =24;//run_t.dispTime_hours --;
					
					
				}
            

             //display hours
                  glcd_t.number5_low =  gpro_t.set_timer_timing_hours / 10 ;
                  glcd_t.number5_high =  gpro_t.set_timer_timing_hours / 10 ;

                  glcd_t.number6_low  = gpro_t.set_timer_timing_hours % 10; //
                  glcd_t.number6_high =  gpro_t.set_timer_timing_hours % 10; //
                   //dispaly minutes 
                  glcd_t.number7_low =  0;
                  glcd_t.number7_high =   0;

                  glcd_t.number8_low = 0;
                  glcd_t.number8_high =   0;

                


                Set_Timer_Timing_Lcd_Blink();
        
       

         break;
 
         }
}

/***************************************************************************
    *
    *Function Name:void Dec_Key_Fun(uint8_t cmd)
    *Function : key down function 
    *Input Ref: run cmd
    *Return Ref : NO
    *
***************************************************************************/
void Add_Key_Fun(uint8_t cmd)
{

 // static uint8_t add_key;
   switch(cmd){
        
    case set_temp_value_item:  //set temperature value 

         gpro_t.gTimer_run_main_fun=0;
         gpro_t.gTimer_run_dht11=0;
        gctl_t.gTimer_compare_ptc_value=0;
        
        gctl_t.gSet_temperature_value   ++;
        if(gctl_t.gSet_temperature_value   < 20){
            gctl_t.gSet_temperature_value  =20;
        }

        if(gctl_t.gSet_temperature_value   > 40)gctl_t.gSet_temperature_value  = 20;

        glcd_t.number1_low = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number1_high = gctl_t.gSet_temperature_value   / 10 ;
        glcd_t.number2_low  =gctl_t.gSet_temperature_value   % 10; //
        glcd_t.number2_high = gctl_t.gSet_temperature_value  % 10; //

         //add_key = 1;
        gkey_t.set_temp_value_be_pressed = 1;
        gctl_t.gTimer_set_temp_value =0;

       
       gctl_t.smart_phone_manual_on_off=0; //unfreeze maybe turn on PTC heat 
      
       // LCD_Disp_Temperature_Value_Handler();
    break;

    case mode_set_timer: //set timer timing value 



         gkey_t.gTimer_disp_set_timer = 0; 

         gpro_t.set_timer_timing_minutes=0;


        	gpro_t.set_timer_timing_hours++ ;//run_t.dispTime_minutes = run_t.dispTime_minutes + 60;
		   if(gpro_t.set_timer_timing_hours > 24){ //if(run_t.dispTime_minutes > 59){

		          gpro_t.set_timer_timing_hours=0;//run_t.dispTime_hours =0;
		                
             }
	    //display hours
        glcd_t.number5_low =  gpro_t.set_timer_timing_hours / 10 ;
        //glcd_t.number5_high =  gpro_t.set_timer_timing_hours / 10 ;
        glcd_t.number5_high = glcd_t.number5_low ;

        glcd_t.number6_low  = gpro_t.set_timer_timing_hours% 10; //
        //glcd_t.number6_high =  gpro_t.set_timer_timing_hours % 10; //
        glcd_t.number6_high =  glcd_t.number6_low;
        //dispaly minutes 
        glcd_t.number7_low =  0;
        glcd_t.number7_high = 0;

        glcd_t.number8_low = 0;
        glcd_t.number8_high =  0;


        Set_Timer_Timing_Lcd_Blink();
         
        
     break;
        
    }

}


void key_add_dec_set_temp_value_fun(void)
{

    // 如果gkey_t结构体中的set_temp_value_be_pressed成员的值为1
if(gkey_t.set_temp_value_be_pressed == 1){
       gkey_t.set_temp_value_be_pressed ++;
        gpro_t.gTimer_run_dht11=0; 
        gctl_t.gTimer_compare_ptc_value=0;
       LCD_Disp_Temperature_Value_Handler();
    }
}
    
 





