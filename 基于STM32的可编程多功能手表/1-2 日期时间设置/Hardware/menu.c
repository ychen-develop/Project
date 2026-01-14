#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "SetTime.h"

uint8_t KeyNum;

void Peripheral_Init(void)
{
	MyRTC_Init();
	Key_Init();
	LED_Init();
}

/*----------------首页时钟--------------*/

void Show_Clock_UI(void)
{
	MyRTC_ReadTime();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}

int clkflag=1;

int First_Page_Clock(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		
		if(KeyNum==1) // 上一项
		{
			clkflag--;
			if(clkflag<=0) clkflag=2;
		}
		else if(KeyNum==2) // 下一项
		{
			clkflag++;
			if(clkflag>=3) clkflag=1;
		}
		else if(KeyNum==3) // 确认
		{
			OLED_Clear();
			OLED_Update();
			return clkflag;
		}
		
		switch(clkflag)
		{
			case 1:
				Show_Clock_UI();
				OLED_ReverseArea(0,48,32,16);
				OLED_Update();
				break;
			case 2:
				Show_Clock_UI();
				OLED_ReverseArea(96,48,32,16);
				OLED_Update();
				break;	
		}
	}
}

/*----------------设置界面--------------*/
void Show_SettingPage_UI(void)
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}

int setflag = 1;
int SettingPage(void)
{
	uint8_t setflag_temp;
	while(1)
	{
		setflag_temp = 0; // 清零
		KeyNum=Key_GetNum();
		
		if(KeyNum==1) // 上一项
		{
			setflag--;
			if(setflag<=0) setflag=2;
		}
		else if(KeyNum==2) // 下一项
		{
			setflag++;
			if(setflag>=3) setflag=1;
		}
		else if(KeyNum==3) // 确认
		{
			OLED_Clear();
			OLED_Update(); // 不需要return
			
			setflag_temp = setflag;
		}
		
		if(setflag_temp==1) {return 0;}
		else if(setflag_temp==2) {SetTime();}
		
		switch(setflag)
		{
			case 1:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			case 2:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();
				break;	
		}
	}
}
