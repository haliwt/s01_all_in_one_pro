#include "bsp_esp8266.h"
#include "bsp.h"

uint8_t *sub_buf;
uint8_t usart2_flag;

static uint32_t ic_id;


/*********************************************************
 *
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
***********************************************************/
uint8_t at_send_data(uint8_t* pdata, uint16_t len)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, pdata, len, 0xffff))
	{
		return len;
	}
	else
	{
		return 0;
	}	
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void auto_init_wifi_mode_handler(void)
{
  
	WIFI_IC_ENABLE();
	if(wifi_t.power_on_login_tencent_cloud_flag ==0){
	   wifi_t.power_on_login_tencent_cloud_flag=1;
	   wifi_t.gTimer_power_first_link_tencent=0;
	  // at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
	   at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
       HAL_Delay(1000);

	}
	if(wifi_t.gTimer_power_first_link_tencent > 1 &&  wifi_t.power_on_login_tencent_cloud_flag==1 ){
	   wifi_t.gTimer_power_first_link_tencent=0;
	    wifi_t.power_on_login_tencent_cloud_flag=2;
	   
	   

	}
}

void auto_link_smartphone_tencent_handler(void)
{
    
  if(wifi_t.power_on_login_tencent_cloud_flag==2){
   	wifi_t.power_on_login_tencent_cloud_flag++;//3
	wifi_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
    wifi_t.wifi_uart_counter=0;
	wifi_t.soft_ap_config_flag =0;
   }
	if(wifi_t.power_on_login_tencent_cloud_flag==3){
		wifi_t.power_on_login_tencent_cloud_flag++; //4
		
       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
       HAL_Delay(1000);
       wifi_t.gTimer_power_first_link_tencent=0;
	}
	if(wifi_t.gTimer_power_first_link_tencent >3 &&  wifi_t.power_on_login_tencent_cloud_flag==4){
		wifi_t.gTimer_power_first_link_tencent=0;
		wifi_t.power_on_login_tencent_cloud_flag=0xff; //5
	

		if(wifi_link_net_state()==1){
			wifi_t.linking_tencent_cloud_doing =0;
	    }

	}

	

}
/****************************************************************************************
*****************************************************************************************/
void ReConnect_Wifi_Net_ATReset_Hardware(void)
{


    if(wifi_t.link_tencent_step_counter==0){
		  wifi_t.gTimer_login_tencent_times=0;
          gpro_t.gTimer_get_data_from_tencent_data=0;
		  wifi_t.link_tencent_step_counter=driver_esp8266_rest;
		   WIFI_IC_DISABLE();
            osDelay(200);//HAL_Delay(300);
           Disip_Wifi_Icon_State();
            osDelay(200);//HAL_Delay(300);
           Disip_Wifi_Icon_State();

            osDelay(200);//HAL_Delay(300);
           Disip_Wifi_Icon_State();
            osDelay(200);
		   
	   }
	
	   if(wifi_t.gTimer_login_tencent_times > 0 && wifi_t.link_tencent_step_counter==driver_esp8266_rest){
		   wifi_t.gTimer_login_tencent_times_0=0;
		   wifi_t.link_tencent_step_counter=driver_esp8266_step_2;
		   WIFI_IC_ENABLE();
		   osDelay(200);//HAL_Delay(200);
           Disip_Wifi_Icon_State();

            osDelay(200);//HAL_Delay(200);
           Disip_Wifi_Icon_State();

            osDelay(200);//HAL_Delay(200);
           Disip_Wifi_Icon_State();
          

		}
		//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		if(wifi_t.link_tencent_step_counter==driver_esp8266_step_2 &&  wifi_t.gTimer_login_tencent_times_0 > 3){
		 wifi_t.link_tencent_step_counter=driver_esp8266_step_3;
         gpro_t.gTimer_get_data_from_tencent_data=0;
		 wifi_t.gTimer_login_tencent_times=0;
         wifi_t.gTimer_login_tencent_times_1=0;
         #if 1
		 at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		//  at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
           osDelay(200);//HAL_Delay(200);
          Disip_Wifi_Icon_State();
           osDelay(200);//HAL_Delay(200);
          Disip_Wifi_Icon_State();

          osDelay(200);//HAL_Delay(200);
          Disip_Wifi_Icon_State();

         wifi_t.gTimer_login_tencent_times_1=0;
         #endif 

		}

        if( wifi_t.gTimer_login_tencent_times_1 > 3 &&  wifi_t.link_tencent_step_counter==driver_esp8266_step_3){
		  
		  // wifi_t.gTimer_login_tencent_times_1 =0;

          wifi_t.link_tencent_step_counter=driver_esp8266_step_4;
          

		}
		
		


}



/****************************************************************************************************
	**
	*Function Name:void Wifi_SoftAP_Config_Handler(void)
	*Function: tensent cloud configuration softAP to connector WIFI
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
void Wifi_SoftAP_Config_Handler(void)
{
 uint8_t *device_massage;
    

    device_massage = (uint8_t *)malloc(128);


   switch (wifi_t.wifi_config_net_lable)
  {

    case wifi_set_restor:
           wifi_t.gTimer_get_beijing_time=0;
           gpro_t.gTimer_get_data_from_tencent_data=0;
        
           ReConnect_Wifi_Net_ATReset_Hardware();//InitWifiModule_Hardware()
	
	       if(wifi_t.link_tencent_step_counter==driver_esp8266_step_4){
             wifi_t.wifi_config_net_lable =wifi_set_cwmode;
		   }
	break;


	 case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
        
           wifi_t.gTimer_get_beijing_time=0;
            gpro_t.gTimer_get_data_from_tencent_data=0;
	
	        if(wifi_t.link_tencent_step_counter==driver_esp8266_step_4 ){
				wifi_t.link_tencent_step_counter =driver_esp8266_step_5;
				
         	   HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 0xffff);
               wifi_t.gTimer_login_tencent_times_2=0;
	        }

			if(wifi_t.gTimer_login_tencent_times_2 > 2){
				wifi_t.gTimer_login_tencent_times_2=0;
				wifi_t.wifi_config_net_lable =wifi_set_softap;
				ic_id = HAL_GetUIDw0();


			}
	       
		   gpro_t.gTimer_get_data_from_tencent_data=0;
	

	 break;

	  case wifi_set_softap:
            WIFI_IC_ENABLE();
          
            wifi_t.gTimer_get_beijing_time=0;
            gpro_t.gTimer_get_data_from_tencent_data=0;
			if(wifi_t.link_tencent_step_counter ==driver_esp8266_step_5){
				wifi_t.link_tencent_step_counter =driver_esp8266_step_6;
				
            sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,ic_id);
			usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
            wifi_t.gTimer_login_tencent_step_3 =0;

			}
			if(wifi_t.gTimer_login_tencent_step_3 > 6){
	  		
             wifi_t.gTimer_login_tencent_step_3 =0;
              wifi_t.wifi_config_net_lable=wifi_set_tcdevreg;
			}
		
            gpro_t.gTimer_get_data_from_tencent_data=0;

       

	 case wifi_set_tcdevreg://dynamic register 0x03

        wifi_t.gTimer_get_beijing_time=0;
        gpro_t.gTimer_get_data_from_tencent_data=0;
      
	     if(wifi_t.link_tencent_step_counter ==driver_esp8266_step_6){
		 	wifi_t.link_tencent_step_counter =driver_esp8266_step_7;
			// wifi_t.linking_tencent_cloud_doing  =1;
		     
		     HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 0xffff); //态注册 
		       wifi_t.gTimer_login_tencent_step_3 =0;

	     }
		 if(   wifi_t.gTimer_login_tencent_step_3  > 6){//6
	            wifi_t.gTimer_login_tencent_step_3  =0;
             wifi_t.wifi_config_net_lable=wifi_set_tcsap;
         }

         gpro_t.gTimer_get_data_from_tencent_data=0;
	 break;


	 case wifi_set_tcsap: //5

         wifi_t.gTimer_get_beijing_time=0;
         gpro_t.gTimer_get_data_from_tencent_data=0;
	 
           if(wifi_t.link_tencent_step_counter ==driver_esp8266_step_7){
		 	wifi_t.link_tencent_step_counter =driver_esp8266_step_8;
			  wifi_t.gTimer_login_tencent_step_3 =0;
              wifi_t.soft_ap_config_flag =1;
            //  wifi_t.linking_tencent_cloud_doing  =1;

	        sprintf((char *)device_massage, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",ic_id);
            usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
             
           }

		   
		  if(   wifi_t.gTimer_login_tencent_step_3 > 3){//1
		   
			    wifi_t.gTimer_login_tencent_step_3  = 0;

			 wifi_t.soft_ap_config_flag =1;
			 wifi_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
			 wifi_t.wifi_uart_counter=0;
			 wifi_t.wifi_config_net_lable=wifi_inquire_register_codes; //WT.EDIT 2024.07.22//wifi_t.wifi_config_net_lable=0xff;
		  }
		 gpro_t.gTimer_get_data_from_tencent_data=0;
			
	 break;

     case wifi_inquire_register_codes: //0x06//WT.EDIT 2024.07.22
       wifi_t.gTimer_get_beijing_time=0;
       gpro_t.gTimer_get_data_from_tencent_data=0;

      #if 0
	   if(wifi_t.gTimer_login_tencent_step_3 > 1 && wifi_t.soft_ap_config_success==0){//wifi_t.gTimer_login_tencent_step_3 > 0 
         wifi_t.gTimer_login_tencent_step_3=0;
         wifi_t.linking_tencent_cloud_doing =1;
		 wifi_t.wifi_uart_counter=0;
        HAL_UART_Transmit(&huart2, "AT+TCPRDINFOSET?\r\n", strlen("AT+TCPRDINFOSET?\r\n"), 0xffff); //动
        wifi_t.wifi_config_net_lable=0xff;//
        gpro_t.gTimer_get_data_from_tencent_data=0;
        }

      #endif 

       
       wifi_t.wifi_config_net_lable=0xff;//
       gpro_t.gTimer_get_data_from_tencent_data=0;


       break;

	}


    

     
  free(device_massage);

 }

/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartPhone_Fun(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void SmartPhone_LinkTencent_Cloud(void)
{
   
    if(wifi_t.soft_ap_config_success==1){

       wifi_t.soft_ap_config_success++;
	   wifi_t.gTimer_login_tencent_net=0;
       gpro_t.gTimer_get_data_from_tencent_data=0;
	    HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
	    HAL_Delay(1000);
	

	}

    if(wifi_t.gTimer_login_tencent_net >3 && (wifi_t.soft_ap_config_success==2)){
	   	  wifi_t.gTimer_login_tencent_times =0;
	     if(wifi_link_net_state()==1){
			   
				wifi_t.get_rx_beijing_time_enable=0;
			    
            }
        
    }
	
}



void wifi_Disconnect_Fun(void)
{

  HAL_UART_Transmit(&huart2, "AT+TCMQTTDISCONN\r\n", strlen("AT+TCMQTTDISCONN\r\n"), 5000);//开始连接

}

void Get_BeiJing_Time_Cmd(void)
{

 // HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", strlen("AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"), 5000);//开始连接
    HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8\r\n", strlen("AT+CIPSNTPCFG=1,8\r\n"), 5000);//开始连接

}

void Get_Beijing_Time(void)
{

   HAL_UART_Transmit(&huart2, "AT+CIPSNTPTIME?\r\n", strlen("AT+CIPSNTPTIME?\r\n"), 5000);//开始连接


}

void PowerOn_Self_Auto_Link_Tencent_Cloud(void)
{

	 uint8_t auto_link_cloud_flag=0;
    

   // device_massage = (uint8_t *)malloc(128);


   switch (auto_link_cloud_flag)
  {

    case 0:
           auto_init_wifi_mode_handler();//InitWifiModule();
        
		   HAL_Delay(1000);

           auto_link_cloud_flag =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
     
            HAL_Delay(1000);
	
			 wifi_t.randomName[0]=HAL_GetUIDw0();
		

	 break;
	 default:
        
	 break;

}
 // free(device_massage);

}




void Wifi_Link_Tencent_Net_State(void)
{

   HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 5000);

  
 
  
}

void Wifi_Reconnect_Tencent_Net(void)
{

   HAL_UART_Transmit(&huart2, "AT+CWJAP\r\n", strlen("AT+CWJAP\r\n"), 5000);

   //AT+CWRECONNCFG?
 //  HAL_UART_Transmit(&huart2, "AT+CWRECONNCFG?\r\n", strlen("AT+CWRECONNCFG?\r\n"), 5000);
 
  
}





