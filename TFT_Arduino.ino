/************************************************* 
 File name: TFT_Arduino.ion// 文件名
 Author: LiShuangLong  
 Version:V1.1 Date: 2023.4.20// 作者、版本及完成日期
 Description: 函数入口程序主程序// 用于详细说明此程序文件完成的主要功能，与其他模块
 Others: 无   // 其它内容的说明
 Function List: void RECD_Date();//串口初始化  void Display_Init();//显示初始化  
 // 主要函数列表，每条记录应包括函数名及功能简要说明
*************************************************/
#include "TFont.h"
#include "wifi.h"

/*
//任务创建及管理函数
#include <TaskScheduler.h>

Scheduler runner;

Task t1(1, TASK_FOREVER, &RECD_Date);
Task t2(2000, TASK_FOREVER, &Tft_Display);


void Task_Init()
{
  runner.init();
  runner.addTask(t1);
  t1.enable();
  runner.addTask(t2);
  t2.enable();
}
*/

void setup()
{
  //Task_Init();//任务管理

  Uart_Init();//串口初始化

  Display_Init();//显示初始化
    
   
}


void loop()
{
  //runner.execute();

  RECD_Date();

  Tft_Display();

}
