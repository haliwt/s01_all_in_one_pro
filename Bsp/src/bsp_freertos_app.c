#include "bsp.h"
//#include "queue.h"



uint32_t mode_key_long_conter;
uint32_t power_key_long_conter;




/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define POWER_LONG_KEY_4         (1 << 4)
#define MODE_LONG_KEY_5          (1 << 5)
///#define RUN_DEC_6           (1 << 6)
//#define RUN_ADD_7           (1 << 7)

#define PHONE_POWER_ON_RX_8       (1<<8)
#define PHONE_POWER_OFF_9         (1<<9)

#define PHONE_POWER_ON_10         (1<<10)

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);



/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
//	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}


/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
   // MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为40ms */
	uint32_t ulValue;
    static uint8_t add_flag,dec_flag,smart_phone_sound,key_mode_sound;
    static uint8_t key_power_long_sound ,power_on_sound,key_mode_long_sound;
    static uint8_t power_sound_flag;
	
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0)
			{
              if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                }
                else{
            
                    power_on_sound = 1;
                    power_key_long_conter=0;
                    gpro_t.gTimer_shut_off_backlight =0;
                }
                     
                
				                                    
			}
            else if((ulValue & PHONE_POWER_ON_RX_8 ) != 0)
            {
                 if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                 else{
                    smart_phone_sound = 1;
                    power_key_long_conter=0;
                    gpro_t.gTimer_shut_off_backlight =0;
                

                }
                       
            }
            else if((ulValue & POWER_LONG_KEY_4) != 0){

               if(gkey_t.key_power == power_on){

              if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

               }
              else{

                   key_power_long_sound =1;
                   mode_key_long_conter = 0;
                   gpro_t.gTimer_exit_mode_long_key =0;
                 }
               }
             }
            else if((ulValue & PHONE_POWER_OFF_9 ) != 0){
               if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                }
                else{
                
                    power_on_sound = 1;
                    power_key_long_conter=0;
                    gpro_t.gTimer_shut_off_backlight =0;
                }
               
            }
            else if((ulValue & MODE_KEY_1) != 0){

               //switch timer timing and works timing 
                if(gkey_t.key_power == power_on ){

                  if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{
                  
                     key_mode_sound =1;
                     power_key_long_conter=0;
                     mode_key_long_conter = 0;
                     gpro_t.gTimer_shut_off_backlight =0;

                    }

                }

            }   
            else if((ulValue & MODE_LONG_KEY_5) != 0){
               
                if(gkey_t.key_power==power_on){


                   if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{
                   
                    key_mode_long_sound= 1;
                    
                   power_key_long_conter =0;
                   gpro_t.gTimer_exit_mode_long_key =0;
                 }

                }
                
            }
            else if((ulValue & DEC_KEY_2) != 0){

                   
                 power_key_long_conter=0;
                 mode_key_long_conter = 0;
                 if(gkey_t.key_power==power_on){

                   if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{
                    dec_flag =1;
                    gpro_t.gTimer_shut_off_backlight =0;


                   }
                  }
               
            }
            else if((ulValue & ADD_KEY_3) != 0){

                 power_key_long_conter=0;
                 mode_key_long_conter = 0;
                  if(gkey_t.key_power==power_on){

                 if(gpro_t.shut_Off_backlight_flag == turn_off){

                     gpro_t.gTimer_shut_off_backlight =0;
                     wake_up_backlight_on();
                     buzzer_sound();

                  }
                  else{

                   add_flag =1;
                   gpro_t.gTimer_shut_off_backlight =0;

                }
                
                }
                 
                    
            }
        }
        else {
            
              
        if(power_sound_flag==0){
         
          power_sound_flag++;
          LED_Mode_Off();
          Backlight_Off();
          LcdDisp_Init();
          
          buzzer_sound();
     
        

        }
        else{

         if(power_on_sound ==1 ||key_power_long_sound ==1 ||key_mode_sound== 1|| key_mode_long_sound==1||add_flag==1 || dec_flag==1 || smart_phone_sound == 1 ){



            if(power_on_sound == 1){
                power_on_sound++;
                power_on_key_handler();
            
                 buzzer_sound();

            }else if(key_power_long_sound ==1){

                  
                  key_power_long_sound++;
                   
                   power_long_key_fun();
                   buzzer_sound();
             }
             else if(key_mode_sound == 1){
                 key_mode_sound++;
                  
                  mode_key_fun();
                  buzzer_sound();
             }
             else if(key_mode_long_sound == 1){

                  key_mode_long_sound++;
                  
                  mode_long_key_fun();
                  buzzer_sound();


             }
             else if(add_flag ==1){
                     add_flag ++;
                      buzzer_sound();
                     osDelay(20);

             }
             else if(dec_flag ==1){
                     dec_flag ++;
                      buzzer_sound();
                     osDelay(20);

             }
             else if(smart_phone_sound == 1){

                   smart_phone_sound++;
             
                  buzzer_sound();
                  smartphone_power_on_handler();

             }
            }

            if(gkey_t.key_power==power_on){


              
                if(add_flag ==2){
                    add_flag ++;
                    Add_Key_Fun(gkey_t.key_add_dec_mode);
                 

                 }
                 else if(dec_flag ==2){

                       dec_flag ++;
                       Dec_Key_Fun(gkey_t.key_add_dec_mode);
                 }
            
              power_on_run_handler();
              key_add_dec_set_temp_value_fun();
              key_mode_be_pressed_send_data_wifi();
             
              Record_WorksOr_Timer_Timing_DonotDisp_Handler();

              if(gkey_t.wifi_led_fast_blink_flag==1 && gctl_t.fan_warning ==0 && gctl_t.ptc_warning == 0){

                  WIFI_Process_Handler();
                  detection_net_link_state_handler();
                  Disip_Wifi_Icon_State();
              
              }
              
              mainboard_active_handler();
              disp_set_temperature_value_handler();
              backlight_on_off_state();
              disp_works_or_timer_timing_fun();
              fan_run_state_handler();
            
              LCD_Timer_Colon_Flicker();
             
              Disip_Wifi_Icon_State();

              Detected_Fan_Works_State();
              Detected_Ptc_Works_State();

              if(gpro_t.gTimer_exit_mode_long_key > 1 && (key_power_long_sound  == 2 || key_mode_long_sound==2)){

                 if(key_power_long_sound ==2){
                     power_key_long_conter =0; //clear power key loong flag .
                     key_power_long_sound = 0;
                       

                  }
                  else if(key_mode_long_sound==2){
                      key_mode_long_sound =0;
                      mode_key_long_conter =0;



                  }



              }

            }
            else {
            
             power_off_run_handler();

            }
            if(gkey_t.wifi_led_fast_blink_flag==0){
                wifi_get_beijint_time_handler();
                MainBoard_Self_Inspection_PowerOn_Fun();
            }
           }
        }
    }
}

/**********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
 
     while(1)
    {
   

      if(KEY_POWER_VALUE()==KEY_DOWN){

        while(KEY_POWER_VALUE() == KEY_DOWN && power_key_long_conter < 896001){

               power_key_long_conter++;
               if(power_key_long_conter > 895000){
                   power_key_long_conter =896009;

                xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                            POWER_LONG_KEY_4,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

                }

         }

        
        if(power_key_long_conter < 895000){
      
            xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
            POWER_KEY_0,      /* 设置目标任务事件标志位bit0  */
            eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        }

       }
       else if(KEY_MODE_VALUE() == KEY_DOWN   ){

        while(KEY_MODE_VALUE() == KEY_DOWN && mode_key_long_conter < 896001 ){
          mode_key_long_conter++;
         if(mode_key_long_conter > 895000){
                 
                  mode_key_long_conter = 896009;
               
               xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         MODE_LONG_KEY_5,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
            }
        }
        if(mode_key_long_conter < 895000){
           xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
               MODE_KEY_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        }

       }
      else if(KEY_ADD_VALUE() == KEY_DOWN){
         xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
                ADD_KEY_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               
     }
     else if(KEY_DEC_VALUE() == KEY_DOWN){
        xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
                DEC_KEY_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                
  
     }
        
     vTaskDelay(20);
    }

 }
  

/**********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
static void AppTaskCreate (void)
{

	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 256,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}




/*********************************************************************
*
*Function Name: 
*Function :
*Input Ref:
*Return Ref:
*
*********************************************************************/
void smartphone_turn_on_handler(void)
{
   
    xTaskNotify(xHandleTaskMsgPro,  /* 目标任务 */
    PHONE_POWER_ON_RX_8,      /* 设置目标任务事件标志位bit0  */
    eSetBits);  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */

}

/*****************************************************************************
 * 
 * Function Name: void App_PowerOff_Handler(void)
 * Function:
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void App_PowerOff_Handler(void)
{
     
     xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
	 PHONE_POWER_OFF_9,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}


