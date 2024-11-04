
// code from 江协
#ifndef __LCD1602_H__
#define __LCD1602_H__

/**
  * @brief LCD1602写指令
  * @param wByte 指令
 */
void LCD1602_WriteCommd(unsigned char wByte);

/**
  * @brief LCD1602写数据
  * @param wByte 数据
 */
void LCD1602_WriteData(unsigned char wByte);

/**
  * @brief 初始化函数
 */
void LCD1602_Init(void);

/**
  * @brief 显示字符
  * @param Row 行，范围`1~2`
  * @param Column 列，范围`1~40`
  * @param wChar 待显示的字符
 */
void LCD1602_DispChar(unsigned char Row,unsigned char Column,unsigned char wChar);

/**
  * @brief 显示字符串
  * @param Row 行，范围`1~2`
  * @param Column 列，范围`1~40`
  * @param wString 待显示的字符串
 */
void LCD1602_DispString(unsigned char Row,unsigned char Column,char wString[]);

/**
  * @brief 显示无符号数字
  * @param Row 行，范围`1~2`
  * @param Column 列，范围`1~40`
  * @param wNum 待显示的无符号数字，范围`0~65535`
  * @param Length 在屏幕上显示的长度，范围`1~16`
 */
void LCD1602_DispUnInt(unsigned char Row, unsigned char Column, unsigned int wNum, unsigned char Length);

/**
 * @brief LCD1602清屏
 */
void LCD1602_Cls();

#endif