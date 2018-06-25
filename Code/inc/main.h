#pragma once

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <main.h>
#include <RCC.h>
#include <BlinkPC13.h>

#include <hd44780.h>

void vTaskLedBlink(void *arg);
void vTaskLCD(void *arg);
