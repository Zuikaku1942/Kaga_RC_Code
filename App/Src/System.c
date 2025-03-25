/*
 * System.c
 *
 *  Created on: Mar 25, 2025
 *      Author: erwin
 */
#include "System.h"

extern UART_HandleTypeDef huart1;
extern uint16_t ADC_VOLTAGE_VALUE ;

void Light_init()//呼吸灯启动
{
    HAL_TIM_PWM_Start( &htim3 , TIM_CHANNEL_1 );//初始化tim3

    while (1){
        for ( int i = 0 ; i < 100 ; i++ ){
            __HAL_TIM_SET_COMPARE( &htim3 ,TIM_CHANNEL_1 , i );//控制比较寄存器，动态调节占空比增大
            HAL_Delay(20);
        }

        for ( int j = 99 ; j > -1 ; j-- ){
            __HAL_TIM_SET_COMPARE( &htim3 ,TIM_CHANNEL_1 , j );//控制比较寄存器，动态调节占空比变小
            HAL_Delay(20);
        }
    }
}

void Get_voltage(void) {//这个是手动测试喵
    ADC_VOLTAGE_MEASURE();
    HAL_Delay(10); // 等待 DMA 完成，仅用于测试
    printf("Voltage sama is : %d\n meow~ ", ADC_VOLTAGE_VALUE);
}