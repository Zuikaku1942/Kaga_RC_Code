/*
 * System.c
 *
 *  Created on: Mar 25, 2025
 *      Author: erwin
 * 
 * 本文件包含系统级功能函数的实现，将基础库中的功能打包成更高级的API
 * 主要功能：
 * 1. 呼吸灯控制 - 通过PWM实现LED亮度渐变效果
 * 2. 手动电压测量 - 提供简单的函数接口进行ADC采样并通过printf输出
 */
#include "System.h"
#include "stdio.h"

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
    /* 
     * 手动电压测量函数
     * 与TIM5自动触发不同，此函数可以在需要时随时调用
     * 调用后会触发一次ADC采样，并将结果通过printf输出到串口
     * 串口配置：2Mbps波特率，通过重定向的printf输出
     */
    ADC_VOLTAGE_MEASURE();
    HAL_Delay(10); // 等待 DMA 完成，仅用于测试
    printf("Voltage sama is : %d\n meow~ ", ADC_VOLTAGE_VALUE);
}