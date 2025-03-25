/*
 * ADC_VOLTAGE.c
 *
 * Created on: Mar 25, 2025
 *      Author: Mikuma
 *本文档实现了：连续电压信号的DMA测量。用全局中断完成1s间隔的自动测量 - 设计通信报文，在完成电压测量后使用UART发出
 *
 *
 * 感谢grok，我叫她帮我写了以下注释：
 * @usage How to call in main function：：
 *        1. 初始化系统和外设：
 *           - HAL_Init();                    // 初始化HAL库
 *           - SystemClock_Config();          // 配置系统时钟
 *           - MX_GPIO_Init();                // 初始化GPIO
 *           - MX_DMA_Init();                 // 初始化DMA（必须在ADC之前）
 *           - MX_ADC1_Init();                // 初始化ADC1
 *           - MX_TIM5_Init();                // 初始化TIM5，配置为1秒中断
 *           - MX_USART1_UART_Init();         // 初始化UART1
 *        2. 启动定时器中断：
 *           - HAL_TIM_Base_Start_IT(&htim5); // 启动TIM5中断，触发自动测量
 *        3. 主循环：
 *           - while(1) {}                    // 主循环保持运行，测量由中断驱动
 *
 * @note
 *        - 确保TIM5配置为1秒中断。
 *        - UART发送的报文格式可根据需要在HAL_ADC_ConvCpltCallback中调整。
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
        printf("Voltage sama is : %d\n meow~", ADC_VOLTAGE_VALUE); // 通过 UART 发送
    }
}

// USART 重定向 printf 到串口
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);  // 通过 USART1 发送字符
    return ch;
}
