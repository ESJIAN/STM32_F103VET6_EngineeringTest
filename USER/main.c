//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PB13（SCL）
//              D1   接PB15（SDA）
//              RES  接PB11
//              DC   接PB10
//              CS   接P12               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "main.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "key.h"
#include "led.h"
#include "Card.H"
#include "string.h"
#include "voice.h"
// u32 TimingDelay;
u8 btemp=0;
unsigned char nfc_data[16]={0};     // Notice:不能换成u8类型数组,否则Debug会进入循环
unsigned char char_nfc_data[16]={0};	//
 int main(void)
 {	u8 t;
	  u8 i;
		delay_init();	    	     //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();			     //LED端口初始化
		OLED_Init();			       //初始化OLED  
		OLED_Clear()  	; 			   //
	 	LED_Init();		  	 	       //初始化与LED连接的硬件接口
		KEY_Init();          	       //初始化与按键连接的硬件接口
		OLED_Clear();
		NFC_Init(115200);
		Voice_modle_Init(9600);   
	
		LED0=0;
		LED1=0;

		while(1)
		{



		//命令处理函数会不断的检查是否有收到命令
		btemp = CommandProcess();
		if(btemp == 0)
		{	// 等待NFC 串口帧接收完毕
			memcpy(nfc_data,Card.BlockData,sizeof(Card.BlockData));
			// 如果读取成功，则把数据帧的数据包拷贝到nfc_data数组中
			for (i = 0; i < 16; i++) 
            {	
                sprintf(char_nfc_data + 2 * i, "%02x", nfc_data[i]);
            }
			// 把NFC读取到的数据块显示到LED上
			OLED_ShowString(0,6,char_nfc_data);
			// Notice1:u8字符组可以直接传入汉字,会自动转换为GBK码
			// 根据数据包的十六进制内容播报不同的提示
			// Notice:不能把下面的nfc_data[]换成char_nfc_data[]，否则无法判断,因为char_nfc_data中的0xB3E0CBAD是字符，而nfc_data中的0xB3E0CBAD是16进制数据
			if (nfc_data[0]==0xB3&&nfc_data[1]==0xE0&&nfc_data[2]==0xCB&&nfc_data[3]==0xAE)
			{
				SendData_to_Voice_modle("赤水",sizeof("赤水"));
			}
			else
			{
				SendData_to_Voice_modle("待添加",sizeof("待添加"));
				SendData_to_Voice_modle("该卡片十六进制内容为",sizeof("该卡片十六进制内容为"));
				SendData_to_Voice_modle(char_nfc_data,sizeof(char_nfc_data));
			}

		}
		




		




			t=KEY_Scan(0);		//得到键值
			switch(t)
			{				 
				case KEY0_PRES:
					LED0=!LED0;
					OLED_Clear();
					OLED_DrawBMP(0,0,128,8,BMP2);
					break;
				case KEY1_PRES:
					LED1=!LED1;
					OLED_Clear();
					OLED_DrawBMP(0,0,128,8,BMP1);
					break;
				case KEY2_PRES:
					LED1=!LED1;
					OLED_Clear();
					OLED_DrawBMP(0,0,128,8,BMP2);
					break;
				case WKUP_PRES:				
					// LED0=!LED0;
					// LED1=!LED1;
					// OLED_Clear();
					// OLED_ShowCHinese(30,0,0);//元
					// OLED_ShowCHinese(48,0,1);//器
					// OLED_ShowCHinese(66,0,2);//科
					// OLED_ShowCHinese(84,0,3);//技
					// OLED_ShowString(0,3,"0.96' OLED TEST");
					// //OLED_ShowString(8,2,"ZHONGJINGYUAN");  
				    // //OLED_ShowString(20,4,"2014/05/01");  
					// OLED_ShowString(0,6,"ASCII:");  

					// OLED_ShowString(63,6,"CODE:");
				
					break;
				default:
					delay_ms(10);	
			} 
		}	 
	
}


/*******************************************************************************
* 函数名        : Delay
* 描述          : 插入一个延时函数
* 输入          : nCount: specifies the delay time length (time base 10 ms).
* 输出          : None
* 返回          : None
*******************************************************************************/
// void Delay(__IO uint32_t nCount)
// {
//   TimingDelay = nCount;
//   while(TimingDelay != 0){}
//   SysTick->VAL = (uint32_t)0x0;
// }