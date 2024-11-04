
#include <Atmel/REGX52.h>
#include <MATH.H>
#include "lib/time.h"
#include "lib/lcd1602.h"

//下面这些有可能需要更改
/***************************************************************/
// 重新命名端口
sbit LCD1602_RS = P3^5;
sbit LCD1602_RW = P3^6;
sbit LCD1602_EN = P3^4;
#define  LCD1602_DB P0

/***************************************************************/

/***************************************************************/

void LCD1602_WriteCommd(unsigned char wByte){// LCD1602写指令
  LCD1602_EN = 0;
  LCD1602_RS = 0;
  LCD1602_RW = 0;
  LCD1602_DB = wByte;
  LCD1602_EN = 1;
  delayMs(1);//延时1ms
  LCD1602_EN = 0;
  delayMs(1);//延时1ms
}

void LCD1602_WriteData(unsigned char wByte){// LCD1602写数据
  LCD1602_EN = 0;
  LCD1602_RS = 1;
  LCD1602_RW = 0;
  LCD1602_DB = wByte;
  LCD1602_EN = 1;
  delayMs(1);//延时1ms
  LCD1602_EN = 0;
  delayMs(1);//延时1ms
  LCD1602_RS = 0;
  delayMs(1);//延时1ms
}

void LCD1602_SetCursor(unsigned char Row,unsigned char Column){
    unsigned char LCD_AC=0x00;
    // 更新坐标位置
    if(Row==1){LCD_AC += (Column-1);}
    else      {LCD_AC += (0x40+Column-1);}
    // 设置光标位置
    LCD1602_WriteCommd(0x80|LCD_AC);
}

void LCD1602_Init(void){// 初始化函数
  LCD1602_WriteCommd(0x38);//八位数据接口，两行显示，`5*7`点阵
  LCD1602_WriteCommd(0x0c);//显示开，光标关，闪烁关
  LCD1602_WriteCommd(0x06);//数据读写操作后，光标自动加一，画面不动
  LCD1602_WriteCommd(0x01);//清屏
}

void LCD1602_DispChar(unsigned char Row,unsigned char Column,unsigned char wChar){//显示单个字符
  // 首先保证行列的位置有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40){
    // 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 发送要显示的数据
    LCD1602_WriteData(wChar);
  }
}

void LCD1602_DispString(unsigned char Row,unsigned char Column,char wString[]){//显示字符串
  unsigned char i=0;
  // 首先保证行列的位置有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40){
    // 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 发送要显示的数据
    while(wString[i]!='\0'){
      LCD1602_WriteData(wString[i]);
      i++;
    }    
  }
}
  
void LCD1602_DispUnInt(unsigned char Row, unsigned char Column,
                       unsigned int wNum, unsigned char Length){//显示无符号数字
  unsigned char i=0;
  unsigned char temp=0;//存储单个位上的数据
  // 首先保证行、列、显示长度有效，才运行函数
  if((Row==1 || Row==2) && Column>=1 && Column<=40 && Length>=1){
    // 1. 更新光标位置
    LCD1602_SetCursor(Row,Column);
    // 2.发送要显示的数据
    // 2.1 超过5位的长度都显示0
    if(Length>16){Length=16;}
    while(Length>5){
      LCD1602_WriteData(0x30);//直接发送'0'的ASCII码
      Length--;
    }
    // 2.2 5位以内的真实数值
    for(i=0;i<Length;i++){
      temp = (wNum/(unsigned int)pow(10,Length-1-i))%10;
      LCD1602_WriteData(0x30|temp);//直接计算ASCII码
    } 
  }
}

void LCD1602_Cls() {
  LCD1602_WriteCommd(0x01);
}