/*
 * ADC_VOLTAGE.c
 *
 * Created on: Mar 25, 2025
 * 注释作者: Mikuma，Grok，ChatGpt
 *本代码作用是：连续电压信号的DMA测量。用全局中断完成30khz的自动测量 - 设计通信报文，在完成电压测量UART发出报文
 *
 * 详细运作逻辑说明：
 * 1. TIM5配置为30kHz的中断频率(84MHz/2800≈30kHz)，每次中断触发ADC采样
 * 2. ADC采样通过DMA直接存入ADC_VOLTAGE_VALUE变量，不占用CPU时间
 * 3. ADC转换完成后自动触发HAL_ADC_ConvCpltCallback回调函数
 * 4. 回调函数组装数据帧("meow"+数据)并通过UART1发送
 * 5. UART波特率设置为2Mbps，足够支持30kHz采样下的1.8Mbps数据传输需求
 * 
 *首先：报文细节：
 * 帧头: "meow" (0x6D 0x65 0x6F 0x77)，用于标识数据包开始
 * 数据: 2字节ADC采样值，高位在前，低位在后
 * 总长度: 6字节/帧
 * 通信速率: 30kHz * 6字节 * 10位 = 1.8Mbps
 *
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
        Statistic_frame[0] = 0x6D; // 'm' - 帧头第1字节
        Statistic_frame[1] = 0x65; // 'e' - 帧头第2字节
        Statistic_frame[2] = 0x6F; // 'o' - 帧头第3字节
        Statistic_frame[3] = 0x77; // 'w' - 帧头第4字节
        Statistic_frame[4] = (ADC_VOLTAGE_VALUE >> 8) & 0xFF; // 数据高字节
        Statistic_frame[5] = ADC_VOLTAGE_VALUE & 0xFF;        // 数据低字节
        HAL_UART_Transmit(&huart1, Statistic_frame, 6, HAL_MAX_DELAY);
    }
}



