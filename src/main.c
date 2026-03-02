/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "stm32f4xx_hal.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void Delay(int t){
	int i;
	for (i=0; i<t; i++);
}

void delay_us(uint16_t period){

  	__HAL_RCC_TIM6_CLK_ENABLE();
  	TIM6->PSC = 83;		// clk = SystemCoreClock /2/(PSC+1) = 1MHz
  	TIM6->ARR = period-1;
  	TIM6->CNT = 0;
  	TIM6->EGR = 1;		// update registers;

  	TIM6->SR  = 0;		// clear overflow flag
  	TIM6->CR1 = 1;		// enable Timer6

  	while (!TIM6->SR);

  	TIM6->CR1 = 0;		// stop Timer6
  	__HAL_RCC_TIM6_CLK_DISABLE();
}

void delay_01ms(uint16_t period){

  	__HAL_RCC_TIM6_CLK_ENABLE();
  	TIM6->PSC = 8399;		// clk = SystemCoreClock /2 /(PSC+1) = 10KHz
  	TIM6->ARR = period-1;
  	TIM6->CNT = 0;
  	TIM6->EGR = 1;		// update registers;

  	TIM6->SR  = 0;		// clear overflow flag
  	TIM6->CR1 = 1;		// enable Timer6

  	while (!TIM6->SR);

  	TIM6->CR1 = 0;		// stop Timer6
  	__HAL_RCC_TIM6_CLK_DISABLE();
}


int a;

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

  // Enable GPIOD clock
  __HAL_RCC_GPIOD_CLK_ENABLE();

  // Configure GPIO pins 12 and 15 as output push-pull
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin   = GPIO_PIN_12 | GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Infinite loop
  while (1)
    {
       // Set GPIO pin 12 and 15
       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_SET);

       // Wait for 1 ms (delay_01ms unit is 0.1 ms, so 10 x 0.1 ms = 1 ms)
       delay_01ms(10);

       // Clear GPIO pin 12 and 15
       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);

       // Wait for 1 ms before repeating
       delay_01ms(10);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
