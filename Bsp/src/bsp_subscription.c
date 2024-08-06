#include "bsp_subscription.h"
#include <string.h>
#include <stdlib.h>
#include "bsp.h"


static void smartphone_app_timer_power_on_handler(void);





uint8_t TCMQTTRCVPUB[40];
uint8_t rx_tencent_num[1];


uint8_t recoder_tx_net_data_counter;

uint8_t temp_num;

uint8_t  rx_data_array[11];

uint8_t rx_tencent_num_buffer[40] ;

uint8_t wind_total_value;





//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
//void Receive_Data_FromCloud_Data(int type, char *str)
//{
//   uint8_t   iNameLen = 0;
//    char  *p_cName = 0, *p_cPos = str;

//       wifi_t.rx_data_len=strlen(str);

//      if (type == JSOBJECT) {
//        /* Get Key */
//        p_cPos = strchr(p_cPos, '"');
//        if (!p_cPos) {
//            return ;
//        }
//        p_cName = ++p_cPos;
//        p_cPos = strchr(p_cPos, '"');
//        if (!p_cPos) {
//            return ;
//        }
//        iNameLen = p_cPos - p_cName;
//      wifi_t.rx_data_name_len=iNameLen;

//        /* Get Value */
//        p_cPos = strchr(p_cPos, ':');
//      
//      
//       }

//   
//   }

 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
        
	     uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
         wifi_t.randomName[0]=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, wifi_t.randomName[0]);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 0xffff); 
         free(device_massage);
   }
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{
      switch(wifi_t.rx_data_state)
      {
 

      case 0:
      
         if((wifi_t.usart2_dataBuf[0]== 'p')  ||wifi_t.usart2_dataBuf[0]=='E' || wifi_t.usart2_dataBuf[0] =='T')//hex :54 - "T" -fixed
            wifi_t.rx_data_state=1; //=1
          else{
               wifi_t.rx_counter=0;
                wifi_t.rx_data_state=0;
            }
            
         break;
      case 1: //#1
             if((wifi_t.usart2_dataBuf[0]== 'a')||wifi_t.usart2_dataBuf[0]=='R' || wifi_t.usart2_dataBuf[0] =='C')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=2; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
         break;
            
        case 2:
            if((wifi_t.usart2_dataBuf[0]== 'r')||wifi_t.usart2_dataBuf[0]=='R' || wifi_t.usart2_dataBuf[0] =='M')    //hex :4B - "K" -fixed
            wifi_t.rx_data_state=3; //=1
         else{
           wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;
        
        case 3:
            if((wifi_t.usart2_dataBuf[0]== 'a')  ||wifi_t.usart2_dataBuf[0]=='O' || wifi_t.usart2_dataBuf[0] =='Q')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=4; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;

      case 4:
       if((wifi_t.usart2_dataBuf[0]== 'm') ||wifi_t.usart2_dataBuf[0]=='R' || wifi_t.usart2_dataBuf[0] =='T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=5; //=1
         else{
           wifi_t.rx_data_state=0;
            wifi_t.rx_counter=0;
         }
            
      break;

      
      case 5:
       if((wifi_t.usart2_dataBuf[0]== 's')||wifi_t.usart2_dataBuf[0]==':' || wifi_t.usart2_dataBuf[0] =='T')    //hex :4B - "K" -fixed
         wifi_t.rx_data_state=6; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
            
      break;

      case 6:
       if((wifi_t.usart2_dataBuf[0]== '"')||wifi_t.usart2_dataBuf[0]=='2'   || wifi_t.usart2_dataBuf[0] =='S'){  //hex :4B - "K" -fixed
         wifi_t.rx_data_state=7; //=1
    	}
		else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
           
      break;

       case 7:
       if((wifi_t.usart2_dataBuf[0]== ':') ||wifi_t.usart2_dataBuf[0]=='0' || wifi_t.usart2_dataBuf[0] =='T'){  //  ||wifi_t.usart2_dataBuf[0]=='"'  ){ //hex :4B - "K" -fixed
         wifi_t.rx_data_state=8; //=1
        }
        else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

		
            
      break;


      case 8:
       if((wifi_t.usart2_dataBuf[0]== '{') ||wifi_t.usart2_dataBuf[0]=='8' || wifi_t.usart2_dataBuf[0] =='A'){ //hex :4B - "K" -fixed
         
         if(wifi_t.usart2_dataBuf[0]=='8') wifi_t.rx_data_state=11;
         else if(wifi_t.usart2_dataBuf[0]=='A') wifi_t.rx_data_state=12;
         else
            wifi_t.rx_data_state=9; //=1
       	}
        else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
            
      break;
         
     case 9:
        
         if(wifi_t.rx_data_success==0){
		 	
            wifi_t.wifi_data[wifi_t.rx_counter] = wifi_t.usart2_dataBuf[0];
            wifi_t.rx_counter++ ;
            


            if(wifi_t.usart2_dataBuf[0]=='}' || wifi_t.usart2_dataBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
                wifi_t.rx_data_success=1;
                wifi_t.rx_data_state=0;
                wifi_t.received_data_from_tencent_cloud = wifi_t.rx_counter;
                wifi_t.rx_counter=0;


            }
           
         }
         else{
			      wifi_t.rx_data_success=0;
        wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
			      wifi_t.received_data_from_tencent_cloud =0;

         }
       
            
      break;

      case 11:

            wifi_t.esp8266_login_cloud_success =0;
            gctl_t.get_beijing_time_success = 0;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;

      break;

      case 12:


       if(wifi_t.usart2_dataBuf[0]== 'T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=13; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;

     case 13:


       if(wifi_t.usart2_dataBuf[0]== 'E')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=14; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;


      case 14:


       if(wifi_t.usart2_dataBuf[0]== ':')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=15; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;


     case 15:


       if(wifi_t.usart2_dataBuf[0]== '0'){   //hex :4B - "K" -fixed
            wifi_t.esp8266_login_cloud_success =0;
            gctl_t.get_beijing_time_success = 0;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
        }
        else if(wifi_t.usart2_dataBuf[0]== '1'){

            wifi_t.esp8266_login_cloud_success =1;
   
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
        }
        else{

          wifi_t.rx_data_state =0;
           wifi_t.rx_counter=0;


        }

     break;
        
    default:
    break;
    }
  
}
	
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{

 if(wifi_t.rx_data_success==1){
         wifi_t.rx_data_success=0;
         wifi_t.link_beijing_times_flag =0;
		
	
     if(wifi_t.received_data_from_tencent_cloud > 22){ //36
	    wifi_t.received_data_from_tencent_cloud=0;
		wifi_t.get_rx_beijing_time_enable=0;
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
		wifi_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
	   // __HAL_UART_CLEAR_OREFLAG(&huart2);
	  
        strncpy((char*)TCMQTTRCVPUB,(char *)wifi_t.wifi_data,40);

    }
	else{


   if(strstr((char *)wifi_t.wifi_data,"open\":0")){
		  wifi_t.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	else if(strstr((char *)wifi_t.wifi_data,"open\":1")){
	   
	   wifi_t.response_wifi_signal_label = OPEN_ON_ITEM;
	}


	
	if(strstr((char *)wifi_t.wifi_data,"ptc\":0")){
            if(power_on_state() ==power_on){
			//	  gctl_t.ptc_flag=0;
	           wifi_t.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    else if(strstr((char *)wifi_t.wifi_data,"ptc\":1")){
            if(power_on_state() ==power_on){
	       //   gctl_t.ptc_flag=1;
			  wifi_t.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
	
    if(strstr((char *)wifi_t.wifi_data,"Anion\":0")){
          if(power_on_state() ==power_on){
	      //   gctl_t.plasma_flag=0;
			wifi_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    else if(strstr((char *)wifi_t.wifi_data,"Anion\":1")){
            if(power_on_state() ==power_on){
         //   gctl_t.plasma_flag=1;
			wifi_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
	
    if(strstr((char *)wifi_t.wifi_data,"sonic\":0")){
            if(power_on_state() ==power_on){
           // gctl_t.ultrasonic_flag=0;
			wifi_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    else if(strstr((char *)wifi_t.wifi_data,"sonic\":1")){
            if(power_on_state() ==power_on){
          //  gctl_t.ultrasonic_flag=1;
			wifi_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }

	
    if(strstr((char *)wifi_t.wifi_data,"state\":1")){
           if(power_on_state() ==power_on){
            //gctl_t.mode_flag=works_time;
			wifi_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}
		  
    }
    else if(strstr((char *)wifi_t.wifi_data,"state\":2")){
            if(power_on_state() ==power_on){
          //  gctl_t.mode_flag=timer_time;
			wifi_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }
			
    }
	
    if(strstr((char *)wifi_t.wifi_data,"temperature")){

	        if(power_on_state() ==power_on){
             rx_tencent_num_buffer[13] = wifi_t.wifi_data[13]; //
             rx_tencent_num_buffer[14] = wifi_t.wifi_data[14];
			wifi_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)wifi_t.wifi_data,"find")){

		 if(power_on_state()==power_on){
            // wifi_t.fin_speed_full_flag = 1;
			wifi_t.response_wifi_signal_label= FAN_ITEM;
           rx_tencent_num_buffer[6]=wifi_t.wifi_data[6];
                 
            rx_tencent_num_buffer[7]=wifi_t.wifi_data[7];
            rx_tencent_num_buffer[8]=wifi_t.wifi_data[8];
		}
	}
 
    }
   }

}


/****************************************************************************
**
   *Function Name:void Json_Parse_Command_Fun(void)
   *Function: parse setment of wifi receive data from tencent 
   *Input Ref: NO
   *Return Ref:NO
   *

*****************************************************************************/
void Json_Parse_Command_Fun(void)
{

  
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	
  
	switch(wifi_t.response_wifi_signal_label){
       
	case OPEN_OFF_ITEM:

       
		wifi_t.esp8266_login_cloud_success=1;
		
        
        MqttData_Publish_SetOpen(0);  
	    osDelay(200);//HAL_Delay(200);//350
            
		if(gkey_t.key_power==power_off){


        }
        else
	        App_PowerOff_Handler();
		wifi_t.response_wifi_signal_label = 0xff;
        
	  break;

	  case OPEN_ON_ITEM:
   
		wifi_t.esp8266_login_cloud_success=1;

		MqttData_Publish_SetOpen(1);  
		osDelay(200);//HAL_Delay(200);//300
		
		if(gkey_t.key_power==power_on){


        }
        else{
		   smartphone_turn_on_handler();
        }

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
	//	wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
		wifi_t.response_wifi_signal_label = 0xff;

	  break;

	  case PTC_ON_ITEM:
	  if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
	    if(ptc_error_state() ==0){
			
		 buzzer_sound()	;
         gctl_t.smart_phone_manual_on_off = 0;
     
		 Ptc_On();
	    // LED_PTC_ICON_ON();
         MqttData_Publish_SetPtc(0x01);
	  	 osDelay(100);//HAL_Delay(100);//350ms
	  	 
	     gctl_t.ptc_flag=1;
		
		}
		
          
		// wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
         wifi_t.response_wifi_signal_label=0xff;
	  	}
	    
	   break;

	  case PTC_OFF_ITEM:
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		 buzzer_sound()	;
       
     
         gctl_t.smart_phone_manual_on_off = 1;
		 Ptc_Off();
	  //   LED_PTC_ICON_OFF();
		 
         MqttData_Publish_SetPtc(0);
		 osDelay(100);//HAL_Delay(100);
	     gctl_t.ptc_flag=0;

		wifi_t.response_wifi_signal_label = 0xff;
		// wifi_t.gTimer_auto_detected_net_state_times=0;
		 	
			 wifi_t.linking_tencent_cloud_doing =0;


	  	}
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
		    Plasma_Off();

			
            MqttData_Publish_SetPlasma(0);
			osDelay(100);//HAL_Delay(100);
           gctl_t.plasma_flag=0;
		//   wifi_t.gTimer_auto_detected_net_state_times=0;
		   	wifi_t.linking_tencent_cloud_doing =0;
		}
       wifi_t.response_wifi_signal_label = 0xff;
		
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
			Plasma_On();
	     
            MqttData_Publish_SetPlasma(1);
		    osDelay(100);//HAL_Delay(50);//350
           gctl_t.plasma_flag=1;
		 
			wifi_t.linking_tencent_cloud_doing =0;
		   
		}
       
	   wifi_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
            buzzer_sound();
			Ultrasonic_Pwm_Stop();
	      //  LED_RAT_ICON_OFF();
            MqttData_Publish_SetUltrasonic(0);
			HAL_Delay(50);//
            gctl_t.ultrasonic_flag=0;
			
	
		
        }
        
	   wifi_t.response_wifi_signal_label=0xff;
		// wifi_t.gTimer_auto_detected_net_state_times=0;
		 	
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case SONIC_ON_ITEM://ultransonic on
	    if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
			//LED_RAT_ICON_ON();
			Ultrasonic_Pwm_Output();
            MqttData_Publish_SetUltrasonic(1);
			osDelay(100);//HAL_Delay(50);
            gctl_t.ultrasonic_flag=1;
		}
      
	   wifi_t.response_wifi_signal_label=0xff;
	//   wifi_t.gTimer_auto_detected_net_state_times=0;
       wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	
   case STATE_TIMER_MODEL_ITEM: //display timer timing of value  don't AI mode
     
        if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
              buzzer_sound();
               gkey_t.key_mode=disp_timer_timing;
               
              gctl_t.ai_flag = 0 ; //timer model
              disp_ai_iocn();
   

              Display_Timer_Timing();
              
              MqttData_Publish_SetState(2); //timer model  = 2
              osDelay(100);//HAL_Delay(50);
             
              
              
              
              
          }
      
         wifi_t.response_wifi_signal_label = 0xff;
  
      
         
        break;
          
        case STATE_AI_MODEL_ITEM: //display works timing --AI-mode
      
           if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
              buzzer_sound();
              gkey_t.key_mode=disp_works_timing;
              gctl_t.ai_flag = 1;//AI mode
         
              disp_ai_iocn();
              Display_Works_Timing();
              MqttData_Publish_SetState(1); //beijing timing = 1
              osDelay(200);//HAL_Delay(50);
             
              
            
            
          }
        
   
             wifi_t.response_wifi_signal_label = 0xff;
         
             wifi_t.linking_tencent_cloud_doing =0;
          break;
   
        case TEMPERATURE_ITEM:
         if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
          
              temp_decade=wifi_t.wifi_data[14]-0x30;
              temp_unit=wifi_t.wifi_data[15]-0x30;
             
              gctl_t.gSet_temperature_value = temp_decade*10 +  temp_unit;
              if(gctl_t.gSet_temperature_value > 40)   gctl_t.gSet_temperature_value=40;
              if(gctl_t.gSet_temperature_value <20 )   gctl_t.gSet_temperature_value=20;
              MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
              osDelay(100);//HAL_Delay(100);//350

              gkey_t.key_add_dec_mode = set_temp_value_item; //set_temp_value_item;
            gkey_t.gTimer_set_temp_value  = 0;
            gpro_t.set_temperature_value_success =1;
        
           gpro_t.gTimer_run_dht11=0; 
           
         
            Disp_SetTemp_Value(gctl_t.gSet_temperature_value );

            
			
			if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

		    	
					gctl_t.ptc_flag =1;
					Ptc_On();
				   // Disp_Dry_Icon();
                    
                     MqttData_Publish_SetPtc(1);
                     osDelay(20);//HAL_Delay(200);

                     gpro_t.gTimer_run_dht11=10;  //at once display sensor of temperature value 
		    	

			}
			else if(gctl_t.gSet_temperature_value <   gctl_t.dht11_temp_value || gctl_t.gSet_temperature_value ==   gctl_t.dht11_temp_value ){
		   		
                    gctl_t.ptc_flag = 0;
			   		Ptc_Off();
                   // Disp_Dry_Icon();
                    
                     MqttData_Publish_SetPtc(0);
                      osDelay(20);//HAL_Delay(200);

                     gpro_t.gTimer_run_dht11=10;  //at once display sensor of temperature value 
			   		
				
			}

		    buzzer_sound();
              
            
               
          }
        wifi_t.response_wifi_signal_label = 0xff;
         
              
       wifi_t.linking_tencent_cloud_doing =0;
        break;
   
        case FAN_ITEM:
          if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
              buzzer_sound();
   
               if(gctl_t.fan_warning ==0){
   
                   wind_hundred =wifi_t.wifi_data[7]-0x30;
                   wind_decade=wifi_t.wifi_data[8]-0x30;
                   wind_unit = wifi_t.wifi_data[9]-0x30;
              
                  if(wind_hundred ==1 && wind_decade==0 && wind_unit==0)    wifi_t.wind_txdata_tencent=100;
                  else
                     wifi_t.wind_txdata_tencent = wind_hundred*10 + wind_decade;
              
           
              MqttData_Publis_SetFan(wifi_t.wind_txdata_tencent);
              osDelay(100);//HAL_Delay(50);//

                   if(wifi_t.wind_txdata_tencent < 34 ){

                         wifi_t.set_wind_speed_value=2;
                        

                     }
                     else if(wifi_t.wind_txdata_tencent> 33 && wifi_t.wind_txdata_tencent < 67){
                         wifi_t.set_wind_speed_value=1;
                         

                     }
                     else{

                        wifi_t.set_wind_speed_value=0;
                        

                     }
             
            
              }
              else{
                   wifi_t.set_wind_speed_value=0;
   
                  MqttData_Publis_SetFan(wifi_t.set_wind_speed_value);
                  osDelay(100);//HAL_Delay(50);
                  //do seomthing 
   
   
              }
              
          }
          
       
          wifi_t.response_wifi_signal_label = 0xff;
         
           wifi_t.linking_tencent_cloud_doing =0;
          break;
   
        case APP_TIMER_POWER_ON_REF :
   
             //wifi_t.set_beijing_time_flag=0;
             wifi_t.link_beijing_times_flag = 0;
             wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
           
             smartphone_app_timer_power_on_handler();
   
            wifi_t.response_wifi_signal_label=0xff;
          
           
            wifi_t.linking_tencent_cloud_doing =0;
   
        break;


   }


   if(wifi_t.response_wifi_signal_label==0xff){
        
        wifi_t.response_wifi_signal_label=0xf0;

//		for(i=0;i<20;i++){
//		   wifi_t.wifi_data[i]=0;
//		   
//
//        }
      memset(wifi_t.wifi_data,'\0',20);
		
	}
}
/*****************************************************************************
	*
	*Function Name:static void smartphone_app_timer_power_on_handler(void)
	*Function:
	*Inpur Ref:
	*Retern Ref:
	*
*****************************************************************************/
static void smartphone_app_timer_power_on_handler(void)
{

   static uint8_t app_step;


    if(app_step==0 ){

		app_step=1;
		if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		wifi_t.smartphone_app_power_on_flag=1;
		}

		if(wifi_t.smartphone_app_power_on_flag==1){

		if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){

		  gctl_t.ptc_flag=1;
          gctl_t.smart_phone_manual_on_off = 0;
          
		}
		else{ //if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){

		     gctl_t.ptc_flag=0;
	         gctl_t.smart_phone_manual_on_off = 1;

		}

		if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){

		gctl_t.ultrasonic_flag=1;

		}
		else{// if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){

		gctl_t.ultrasonic_flag=0;

		}

		if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
		gctl_t.plasma_flag=1;
		}
		else{ // if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
		gctl_t.plasma_flag=0;
		}



		}
    }
   

   

	if(app_step==1){
	  
		//buzzer_sound();

		//Device_Action_Publish_Handler();
		if(gkey_t.key_power == power_off ){
			wifi_t.smartphone_app_power_on_flag=1;
            wifi_t.link_tencent_thefirst_times=0;
            wifi_t.set_wind_speed_value=0;
            smartphone_turn_on_handler();
		}
        
		
		if(gctl_t.ptc_flag ==0)gpro_t.app_ptc_flag = 1;

        MqttData_Publis_App_PowerOn_Ref();
  
		
		HAL_Delay(200);//
		app_step=0;

	

	}


    
}
      


/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_Link_Net_InputInfo_Handler(void)
{
    
           if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)wifi_t.wifi_data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		wifi_t.soft_ap_config_success=1;
				}
               else if(strstr((const char*)wifi_t.wifi_data,"+TCMQTTCONN:OK")){

				     wifi_t.repeat_login_tencent_cloud_init_ref=0;
	                 wifi_t.esp8266_login_cloud_success=1;
                     gkey_t.wifi_led_fast_blink_flag=0;
				  
				  //link to tencent cloud is success .
	              wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
				
				  wifi_t.soft_ap_config_flag=0;
				 
				  
			  }
              else if(strstr((char*)wifi_t.wifi_data,"+CME ERROR:208")){
						
					       wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
						   wifi_t.esp8266_login_cloud_success =0;
						
						   gkey_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31
						
						
						   wifi_t.get_rx_beijing_time_enable=0;
					   
		   
			   }
              else if(strstr((char*)wifi_t.wifi_data,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

                     wifi_t.serch_for_wifi_flag=1;
                     wifi_t.gTime_link_time_start =0;

              }
              else  if(strstr((char*)wifi_t.wifi_data,"+TCMQTTCONN:FAIL,202")){

                        wifi_t.esp8266_login_cloud_success =0;
                        gkey_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31
                        
                      wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                      wifi_t.get_rx_beijing_time_enable=0;
              }
           
           }
		   else{

		     if(strstr((const char*)wifi_t.wifi_data,"+TCMQTTCONN:OK")){
			 	
                 
				  wifi_t.esp8266_login_cloud_success=1;
			      wifi_t.repeat_login_tencent_cloud_init_ref=0;
				
	              wifi_t.linking_tencent_cloud_doing=0;
			      gkey_t.wifi_led_fast_blink_flag=0;
		
				  wifi_t.soft_ap_config_flag=0;
				 
			  }
              else if(strstr((char*)wifi_t.wifi_data,"+CME ERROR:208")){
				
			
				 wifi_t.esp8266_login_cloud_success =0;
			     wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
			     wifi_t.get_rx_beijing_time_enable=0;
			
             }
              else  if(strstr((char*)wifi_t.wifi_data,"+TCMQTTCONN:FAIL,202")){

                        wifi_t.esp8266_login_cloud_success =0;
                       gkey_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31
                        
                      wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                          
                      wifi_t.get_rx_beijing_time_enable=0;
              }
              else  if(strstr((char*)wifi_t.wifi_data,"+TCMQTTDISCON")){
          
                      wifi_t.esp8266_login_cloud_success =0;
                       wifi_t.get_rx_beijing_time_enable=0;
                        wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
              }
              else  if(strstr((char*)wifi_t.wifi_data,"+TCSAP:WIFI_CONNECT_FAILED")){
          
                      wifi_t.esp8266_login_cloud_success =0;
                      wifi_t.get_rx_beijing_time_enable=0;
                      wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                    
              }

			 


		  }
       
         wifi_t.wifi_uart_counter=0;

         wifi_t.once_rx_data_done = 1;
         
         
        
            
}


void clear_rx_copy_data(void)
{
   uint8_t i;

   if( wifi_t.once_rx_data_done == 1 && wifi_link_net_state() == 0){

      wifi_t.once_rx_data_done ++;

      for(i=0;i<150;i++){
        
       wifi_t.wifi_data[i]=0;
      // wifi_t.auto_det_data[i]=0;
      }
    }
}

/***********************************************************************
    *
    *Function Name:
    *Function:
    *Input Ref: NO
    *Return Ref:NO
    *
***********************************************************************/
void detection_net_link_state_handler(void)
{
    
    if( gkey_t.wifi_led_fast_blink_flag==1 && wifi_t.soft_ap_config_success==0 &&   wifi_t.serch_for_wifi_flag ==1){

       if(wifi_t.gTime_link_time_start  > 40){
           wifi_t.gTime_link_time_start =0;
            wifi_t.serch_for_wifi_flag =0;
           wifi_t.soft_ap_config_success=1;
           

       }

     }

     if(wifi_t.gTime_link_time_start  > 45 &&   wifi_t.serch_for_wifi_flag ==1){

            wifi_t.serch_for_wifi_flag =0;

      }

}


