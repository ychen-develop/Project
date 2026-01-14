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

/*----------------滑动菜单界面--------------*/

uint8_t pre_selection;//上次选择的选项
uint8_t target_selection;//目标选项
uint8_t x_pre=48;//上次选项的x坐标
uint8_t Speed=4;//速度
uint8_t move_flag;//开始移动的标志位，1表示开始移动，0表示停止移动

void Menu_Animation(void)
{
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);
	
	if(pre_selection<target_selection)
	{
		x_pre-=Speed;
		if(x_pre==0)
		{
			pre_selection++;
			move_flag=0;
			x_pre=48;
		}
	}
	
	if(pre_selection>target_selection)
	{
		x_pre+=Speed;
		if(x_pre==96)
		{
			pre_selection--;
			move_flag=0;
			x_pre=48;
		}
	}
	
	if(pre_selection>=1)
	{
		OLED_ShowImage(x_pre-48,16,32,32,Menu_Graph[pre_selection-1]);
	}
	
	if(pre_selection>=2)
	{
		OLED_ShowImage(x_pre-96,16,32,32,Menu_Graph[pre_selection-2]);
	}
	
	OLED_ShowImage(x_pre,16,32,32,Menu_Graph[pre_selection]);
	OLED_ShowImage(x_pre+48,16,32,32,Menu_Graph[pre_selection+1]);
	OLED_ShowImage(x_pre+96,16,32,32,Menu_Graph[pre_selection+2]);
	
	OLED_Update();
}

void Set_Selection(uint8_t move_flag,uint8_t Pre_Selection,uint8_t Target_Selection)
{
	if(move_flag==1)
	{
		pre_selection=Pre_Selection;
		target_selection=Target_Selection;
		Menu_Animation();
	}
}

uint8_t menu_flag=1;
int Menu(void)
{
	uint8_t menu_flag_temp;
	
	move_flag=1;
	uint8_t DirectFlag=2;//置1：移动到上一项；置2：移动到下一项
	
	while(1)
	{
		menu_flag_temp = 0; // 清零
		KeyNum=Key_GetNum();
		
		if(KeyNum==1)//上一项
		{
			DirectFlag=1;
			move_flag=1;
			menu_flag--;
			if(menu_flag<=0)menu_flag=7;
		}
		else if(KeyNum==2)//下一项
		{
			DirectFlag=2;
			move_flag=1;
			menu_flag++;
			if(menu_flag>=8)menu_flag=1;
		}
		else if(KeyNum==3) // 确认
		{
			OLED_Clear();
			OLED_Update(); // 不需要return
			
			menu_flag_temp = menu_flag;
		}
		
		if(menu_flag_temp==1) {return 0;}
		else if(menu_flag_temp==2) {}
		else if(menu_flag_temp==3) {}
		else if(menu_flag_temp==4) {}
		else if(menu_flag_temp==5) {}
		else if(menu_flag_temp==6) {}
		else if(menu_flag_temp==7) {}
		
		if(menu_flag==1)
		{
			if(DirectFlag==1)Set_Selection(move_flag,1,0);
			else if(DirectFlag==2)Set_Selection(move_flag,0,0);
		}
		
		else
		{
			if(DirectFlag==1)Set_Selection(move_flag,menu_flag,menu_flag-1);
			else if(DirectFlag==2)Set_Selection(move_flag,menu_flag-2,menu_flag-1);
		}
	}
}
