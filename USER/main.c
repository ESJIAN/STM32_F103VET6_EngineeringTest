//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PB13��SCL��
//              D1   ��PB15��SDA��
//              RES  ��PB11
//              DC   ��PB10
//              CS   ��P12               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#include"main.h"
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "key.h"
#include "led.h"
#include "Card.H"
#include"string.h"
u32 TimingDelay;
u8 bTemp;
unsigned char nfc_data[16];
 int main(void)
 {	u8 t;
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();			     //LED�˿ڳ�ʼ��
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	; 
	 	LED_Init();		  	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
		KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
		OLED_Clear();
    //		LED_ON;
	// 	OLED_ShowCHinese(30,0,0);//Ԫ
	// 	OLED_ShowCHinese(48,0,1);//��
	// 	OLED_ShowCHinese(66,0,2);//��
	// 	OLED_ShowCHinese(84,0,3);//��
	// 	OLED_ShowString(0,3,"0.96' OLED TEST");
	// 	//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	//  //	OLED_ShowString(20,4,"2014/05/01");  
	// 	OLED_ShowString(0,6,"ASCII:");  
	// 	OLED_ShowString(63,6,"CODE:");





	
		LED0=0;
		LED1=0;
		while(1)
		{
		//��������᲻�ϵļ���Ƿ����յ�����
		bTemp = CommandProcess();
		if(bTemp == 0)
		{
			memcpy(nfc_data,Card.BlockData,sizeof(Card.BlockData));
		}



			t=KEY_Scan(0);		//�õ���ֵ
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
					LED0=!LED0;
					LED1=!LED1;
					OLED_Clear();
					OLED_ShowCHinese(30,0,0);//Ԫ
					OLED_ShowCHinese(48,0,1);//��
					OLED_ShowCHinese(66,0,2);//��
					OLED_ShowCHinese(84,0,3);//��
					OLED_ShowString(0,3,"0.96' OLED TEST");
					//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
				 //	OLED_ShowString(20,4,"2014/05/01");  
					OLED_ShowString(0,6,"ASCII:");  

					OLED_ShowString(63,6,"CODE:");
				
					break;
				default:
					delay_ms(10);	
			} 
		}	 
	
}

/*******************************************************************************
* ������        : Delay
* ����          : ����һ����ʱ����
* ����          : nCount: specifies the delay time length (time base 10 ms).
* ���          : None
* ����          : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;
  while(TimingDelay != 0){}
  SysTick->VAL = (uint32_t)0x0;
}