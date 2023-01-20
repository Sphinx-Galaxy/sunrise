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
#define LEDC_GREEN_CHANNEL		LEDC_CHANNEL_1
#define LEDC_BLUE_CHANNEL		LEDC_CHANNEL_2

enum Mode {Normal, Sweep};

static struct led_stripe_settings_t {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	
	uint8_t speed;
	
	//enum Mode mode;
} led_stripe_settings = {0, 0, 0, 0};

static struct led_stripe_t {
	struct led_stripe_settings_t settings;
	
	void (*set_red)(uint8_t);
	void (*set_green)(uint8_t);
	void (*set_blue)(uint8_t);
	void (*set_speed)(uint8_t);
} led_stripe;

void init_led_stripe();
void init_led(int gpio, int channel);

void set_red(uint8_t value);
void set_green(uint8_t value);
void set_blue(uint8_t value);
void set_speed(uint8_t value);

//void set_mode(enum Mode mode); 
