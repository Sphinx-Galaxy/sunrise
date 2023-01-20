/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation see led_stripe.h
 *
 */
 
#include "led_stripe.h"

void init_led_stripe() {
	led_stripe.settings = led_stripe_settings;

	init_led(0, LEDC_RED_CHANNEL);

	led_stripe.set_red = &set_red;
	led_stripe.set_green = &set_green;
	led_stripe.set_blue = &set_blue;
	led_stripe.set_speed = &set_speed;
}
 
void init_led(int gpio, int channel)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = channel,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = gpio,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void set_red(uint8_t value) {
	led_stripe_settings.red = value;
	
	ledc_set_duty(LEDC_MODE, LEDC_RED_CHANNEL, value);
	ledc_update_duty(LEDC_MODE, LEDC_RED_CHANNEL);
}

void set_green(uint8_t value) {
	led_stripe_settings.green = value;
	
	// Set PWM
}

void set_blue(uint8_t value) {
	led_stripe_settings.blue = value;
	
	// Set PWM
}

void set_speed(uint8_t value) {
	led_stripe_settings.speed = value;
	
	// Set PWM
}
