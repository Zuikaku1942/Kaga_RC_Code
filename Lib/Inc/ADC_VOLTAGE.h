/*
 * ADC_VOLTAGE.h
 *
 *  Created on: Mar 25, 2025
 *      Author: erwin
 */
#ifndef INC_ADC_VOLTAGE_C_
#define INC_ADC_VOLTAGE_C_

#include "System.h"


void ADC_VOLTAGE_MEASURE();
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
int fputc(int ch, FILE *f);



#endif /* INC_ADC_VOLTAGE_C_ */

