#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Timer.h"
#include "Key.h"
#include "dino.h"

/**
  * 坐标轴定义：
  * 左上角为(0, 0)点
  * 横向向右为X轴，取值范围：0~127
  * 纵向向下为Y轴，取值范围：0~63
  * 
  *       0             X轴           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	OLED_Clear();
	
	/*外设初始化*/
	Peripheral_Init();
	
	/*定时器初始化*/
	Timer_Init();
	
	int clkflag1;

	while (1)
	{
		clkflag1=First_Page_Clock();
		if(clkflag1==1) {Menu();} // 菜单
		else if(clkflag1==2) {SettingPage();}; // 设置
		
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		StopWatch_Tick();
		Dino_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
