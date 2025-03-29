//
// Created by erwin on 2025/3/29.
//
#include "UART_FREQUENT.h"

extern UART_HandleTypeDef huart1;//已经在main中定义了

#define Voltage_Buffer_Size 6//定义储存缓存区的大小，避免魔法数字
#define Frequent_Analyze_Buffer_Size 50

uint16_t Voltage_Index = 0 ;//做分析数组
uint8_t VOLTAGE_Stock_BUFFER[Voltage_Buffer_Size];//定义存储单次电压的缓存区,用以解析帧头
uint8_t Voltage_Array[Frequent_Analyze_Buffer_Size];


volatile uint8_t UART1_READ_FLAG = 0 ;  //定义读取完成标志位



void External_Uart_Init(){
    HAL_UART_Receive_DMA( &huart1 , VOLTAGE_Stock_BUFFER ,Voltage_Buffer_Size );//启动初始化
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//第一次进入中断模式等待读取
}

void Mikuma_HAL_UART_IRQHandler(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {  // 确保是你的UART实例
        // 检查是否为空闲中断
        if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
            __HAL_UART_CLEAR_IDLEFLAG(huart);  // 清除空闲中断标志
            // 停止DMA接收，防止数据覆盖
            HAL_UART_DMAStop(huart);


            UART1_READ_FLAG = 1;  // 设置接收完成标志

            Parse_UART_Frame();  // 调用解析函数
            // 重启DMA接收，准备下一次数据
            HAL_UART_Receive_DMA(&huart1 , VOLTAGE_Stock_BUFFER ,Voltage_Buffer_Size);
        }
    }
}

void Parse_UART_Frame() {
    uint16_t voltage_value = 0 ;
    if (VOLTAGE_Stock_BUFFER[0] == 0x6D && VOLTAGE_Stock_BUFFER[1] == 0x65
        && VOLTAGE_Stock_BUFFER[2] == 0x6F && VOLTAGE_Stock_BUFFER[3] == 0x77) //判断是否帧头对不对
        voltage_value = (VOLTAGE_Stock_BUFFER[4] << 8) | VOLTAGE_Stock_BUFFER[5];
    if (voltage_value > 4000) { // 接近 3.3V，记为 1（阈值可调整）
        Voltage_Array[Voltage_Index] = 1;
    } else { // 低电平或中间值，记为 0
        Voltage_Array[Voltage_Index] = 0;
    }
    Voltage_Index++;
    if (Voltage_Index >= Frequent_Analyze_Buffer_Size){
        Frequent_analyze();
        Voltage_Index = 0;
        for (uint16_t i = 0; i < Frequent_Analyze_Buffer_Size; i++) {
            Voltage_Array[i] = 0;
        }
    }

    UART1_READ_FLAG = 0;
}

    void Frequent_analyze(){

}


