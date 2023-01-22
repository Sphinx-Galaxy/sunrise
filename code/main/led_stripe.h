/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation
 *
 */
 
#pragma once

#include <stdint.h>
#include "driver/ledc.h"

#define LEDC_TIMER				LEDC_TIMER_0
#define LEDC_MODE				LEDC_LOW_SPEED_MODE
#define LEDC_DUTY_RES			LEDC_TIMER_13_BIT
#define LEDC_FREQUENCY			(5000)

#define LEDC_RED_CHANNEL		LEDC_CHANNEL_0
#define LEDC_RED_GPIO			(18)
#define LEDC_GREEN_CHANNEL		LEDC_CHANNEL_1
#define LEDC_GREEN_GPIO			(21)
#define LEDC_BLUE_CHANNEL		LEDC_CHANNEL_2
#define LEDC_BLUE_GPIO			(23)

void init_led_stripe();
void init_led(int gpio, int channel);

void set_red_led_duty(uint8_t value);
void set_green_led_duty(uint8_t value);
void set_blue_led_duty(uint8_t value);
