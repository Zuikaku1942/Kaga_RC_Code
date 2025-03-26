/*
 * ADC_VOLTAGE.c
 *
 * Created on: Mar 25, 2025
 * 注释作者: Mikuma，Grok，ChatGpt
 *本代码作用是：连续电压信号的DMA测量。用全局中断完成30khz的自动测量 - 设计通信报文，在完成电压测量UART发出报文
 *
 *首先：报文细节：
 * How to call in main function？
 *    启动定时器中断即可：
 *    HAL_TIM_Base_Start_IT(&htim5); // 启动TIM5中断，触发自动测量
 *    主循环按兵不动：
 *    while(1) {}                    // 主循环保持运行，测量由中断驱动
 *
 */

#include "ADC_VOLTAGE.h"
//以下代码是ADC采样的基础模块代码，我们将在APP/INC/SYSTEM里用函数把他们打包

extern uint16_t ADC_VOLTAGE_VALUE ;//用外部变量在函数间传递电压值

void ADC_VOLTAGE_MEASURE() { // 启动 DMA 传输 ADC 数据
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_VOLTAGE_VALUE, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){ //tim5周期中断回调函数，当TIM5触发时调用ADC_VOLTAGE_MEASURE进行测量
    if(htim->Instance == TIM5)
        ADC_VOLTAGE_MEASURE();
}

// DMA 完成回调，输出采样值
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        uint8_t Statistic_frame[6];
        Statistic_frame[0] = 0x6D; // 'm'
        Statistic_frame[1] = 0x65; // 'e'
        Statistic_frame[2] = 0x6F; // 'o'
        Statistic_frame[3] = 0x77; // 'w'
        Statistic_frame[4] = (ADC_VOLTAGE_VALUE >> 8) & 0xFF; // 数据高字节
        Statistic_frame[5] = ADC_VOLTAGE_VALUE & 0xFF;        // 数据低字节
        HAL_UART_Transmit(&huart1, Statistic_frame, 6, HAL_MAX_DELAY);
    }
}



