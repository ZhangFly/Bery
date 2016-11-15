#ifndef SENSOR_H_
#define SENSOR_H_

#include "stm32f10x.h"
#include "scan.h"

void Sensor_Init(void);                         /* 初始化传感器控制IO口 */
void Sensor_Select_X(const u8 x);               /* 选择X坐标 */
void Sensor_Select_Y(const u8 y);               /* 选择Y坐标 */
void Sensor_DeSelect_X(const u8 x);             /* 取消选择X坐标 */
void Sensor_DeSelect_Y(const u8 y);             /* 取消选择Y坐标 */
void Sensor_Select(const u8 x, const u8 y);     /* 选择坐标（X,Y）*/
void Sensor_DeSelect(const u8 x, const u8 y);   /* 取消选择坐标（X,Y）*/
u16 Sensor_Get_Code(void);                       /* 获取当前选择的地址 */

Scan* Sensor_Create_Scan(void);
void Sensor_Destroy_Scan(Scan*);

#endif /* SENSOR_H_ */
