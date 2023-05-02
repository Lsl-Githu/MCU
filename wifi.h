#ifndef _wifi_h
#define _wifi_h
/************************************************* 
 File name: wifi.h// 文件名
 Author: LiShuangLong  
 Version:V1.1 Date: 2023.4.20// 作者、版本及完成日期
 Description: wifi连接和接收数据相关程序// 用于详细说明此程序文件完成的主要功能，与其他模块
 Others: 无   // 其它内容的说明
 Function List: void Send_AT(char condition);//发送AT指令函数  
 void RECD_Date();//串口数据接收处理函数
 // 主要函数列表，每条记录应包括函数名及功能简要说明
*************************************************/

#include <ArduinoJson.h> //引用ArduinoJson库
//连接网络
/****软串口头文件****/
#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial mySerial(8,9);

char COUNT_FLAG = 0;/*接收模式切换*/


char xs_flag = 0;/*显示信号*/
const char *KW;/*取json的口味字符串*/
char XS_fg = 0;/*串口空闲开关*/

StaticJsonDocument<200> doc; //声明一个JsonDocument对象


struct REcv
{
  int Id;//编号
  int num;//数量
  String KWrr;//口味

}REcv_Data[8];

/***串口重定向printf***/
char serial_putc( char c, struct __file * )
{

  Serial.write(c);
  return c;
}

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}


//发AT过程
void Send_AT(char condition)
{
  switch (condition) 
  {
    case 0:mySerial.print(F("AT\r\n"));break;  
    case 1:mySerial.print(F("AT+RST\r\n"));break;
    case 2:mySerial.print(F("ATE0\r\n")); break;
    case 3:mySerial.print(F("AT+CWMODE=1\r\n")); break;
    case 4:mySerial.print(F("AT+CWJAP=\"nova\",\"12345678\"\r\n")); break;
    case 5:mySerial.print(F("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n")); break;
    case 6:mySerial.print(F("AT+MQTTUSERCFG=0,1,\"NULL\",\"Test_DC&a1WZaCEwkny\",\"82b47a50c141368ca0e714a1c51d86c7fa4fe6f8429a71760d2b0d3a2c65ffc1\",0,0,\"\"\r\n")); break;  
    case 7:mySerial.print(F("AT+MQTTCLIENTID=0,\"a1WZaCEwkny.Test_DC|securemode=2\\,signmethod=hmacsha256\\,timestamp=1681737890659|\"\r\n"));break;
    case 8:mySerial.print(F("AT+MQTTCONN=0,\"a1WZaCEwkny.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1\r\n"));break;
    case 9:mySerial.print(F("AT+MQTTSUB=0,\"/a1WZaCEwkny/Test_DC/user/get\",0\r\n"));COUNT_FLAG=0;break;
  }

}


/*串口接收数据处理*/
void RECD_Date()
{

  static char COUNT = 0;//控制AT发送的速度
  char terminatorChar = '}';//字符定义
  static char State = 0; //接和收状态切换
  static char Seend_ata = 0;//AT指令的步骤
  static  String ARR = "";//接收缓冲区
  static int xs_cont=0;//*显示延时
  int commaposition = 0;    //查询字符位置
  int commaposition2 = 0;
  int XS = 0;//反序列化获取菜编号
  
  switch (State)
  {
    case 0:
    {
      COUNT++;
      if (COUNT >= 6)
      {
        COUNT = 0;
        Send_AT(Seend_ata);  
        State = 1;
      }
      COUNT_FLAG = 1;

    }break;
  
    case 1:
    {
      if (COUNT_FLAG)
      { 
        if (mySerial.available())
        {
     
          ARR = mySerial.readString();//读取串口数据
          printf("len = %d\n", ARR.length());
          Serial.println(ARR); 

          if ((ARR.charAt(ARR.length() - 4) == 'O' && ARR.charAt(ARR.length() - 3) == 'K') 
            || (ARR.charAt(10) == 'O' && ARR.charAt(11) == 'K') ||(ARR.charAt(0) == '+' && ARR.charAt(1) == 'M'))
          {
            Serial.println(F("CONNET SUCC"));
            COUNT_FLAG = 0; 
            if (Seend_ata < 9)
            {
              Seend_ata++; 
              State = 0;
            } 
            else
            {
              State = 1; 
            }  
          }
          else
          {
            Serial.println(F("CONNET ERRO")); 
          }
      
        }
        
        ARR = "";
        
      }

      else
      {
        if (mySerial.available() > 0)
        {
          ARR = mySerial.readString();
          if (ARR.length() > 0)
          {
            //Serial.println(ARR); 
            //{"kw3":"中辣","flag3":1,"xs":3,"num3":3}
            commaposition = ARR.indexOf('{');//找到字符串中逗号出现的位置
            commaposition2 = ARR.indexOf('}');//找到字符串中逗号出现的位置
            Serial.println(ARR.substring(commaposition, commaposition2 + 1));
            String myJson = ARR.substring(commaposition, commaposition2 + 1);
            DeserializationError error = deserializeJson(doc, myJson); //反序列化JSON数据
            if (!error) //检查反序列化是否成功
            {
              XS = doc["xs"];
              switch (XS)
              {
                case 1:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num1"];
                  KW=doc["kw1"];
                  REcv_Data[XS-1].KWrr = KW;
                
                }break;
                
                case 2:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num2"];
                  KW = doc["kw2"];
                  REcv_Data[XS-1].KWrr = KW;

                }break;
                
                case 3:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num3"];
                  KW = doc["kw3"];
                  REcv_Data[XS-1].KWrr = KW;
                }break;
                
                case 4:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num4"];
                  KW = doc["kw4"];
                  REcv_Data[XS-1].KWrr = KW;
                }break;  
                                
                case 5:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num5"];
                  KW = doc["kw5"];
                  REcv_Data[XS-1].KWrr = KW;
                }break;  
                
                case 6:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num6"];
                  KW = doc["kw6"];
                  REcv_Data[XS-1].KWrr = KW;
                }break; 
                
                case 7:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num7"];
                  KW = doc["kw7"];
                  REcv_Data[XS-1].KWrr = KW;
                }break;   
                            
                case 8:
                {
                  //读取json节点
                  REcv_Data[XS-1].Id = doc["xs"];
                  REcv_Data[XS-1].num = doc["num8"];
                  KW = doc["kw8"];
                  REcv_Data[XS-1].KWrr = KW;
                }break;          
              }
              Serial.println(F("succ:")); 
              myJson = "";  
            }
            ARR = "";
          }

          XS_fg = 0;
          xs_cont = 0;

       }
       else
       {
          if (XS_fg == 0)
          {
            xs_cont++;
          }
          if (xs_cont >= 15)
          {
            xs_cont = 0;
            xs_flag = 1;          
          }
        }
      

      }
  
    }break; 
       
  }
 
}


/*初始化串口*/
void Uart_Init()
{
  printf_begin();
  mySerial.begin(9600);
  Serial.begin(9600);
}


#endif
