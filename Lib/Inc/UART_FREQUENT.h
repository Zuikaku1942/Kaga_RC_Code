//
// Created by erwin on 2025/3/29.
//

#ifndef RC_MISSION_UART_FREQUENT_H
#define RC_MISSION_UART_FREQUENT_H

#include "System.h"

void External_Uart_Init();
void Mikuma_HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void Parse_UART_Frame();
void Frequent_analyze();







#endif //RC_MISSION_UART_FREQUENT_H
