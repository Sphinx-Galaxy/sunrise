/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation see led_stripe.h
 *
 */
 
#include "led_stripe.h"

#include "esp_err.h"
#include "esp_log.h"

static const char* TAG = "LED_STRIPE";

void init_led_stripe() {
	init_led(LEDC_RED_GPIO, LEDC_RED_CHANNEL);
	init_led(LEDC_GREEN_GPIO, LEDC_GREEN_CHANNEL);
	init_led(LEDC_BLUE_GPIO, LEDC_BLUE_CHANNEL);
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

void set_led_on(bool led_on[LEDC_SIZE]) {
	for(uint8_t i = 0; i < LEDC_SIZE; ++i) {
		ESP_LOGI(TAG, "Set LED %d %s", i, (led_on[i] ? "on" : "off"));
		led_stripe.led_on[i] = led_on[i];
	}
	
	set_led_duty(led_stripe.led_duty);
}

void set_led_duty(uint8_t led_duty[LEDC_SIZE]) {	
	for(uint8_t i = 0; i < LEDC_SIZE; ++i) {
		led_stripe.led_duty[i] = led_duty[i];
		ESP_LOGI(TAG, "Set LED %d duty %d", i, led_duty[i]);	
		
		if(led_stripe.led_on[i]) {
			ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, 
										  led_stripe.led_channel[i], 
										  8191.0 * led_stripe.led_duty[i] / 100.0));
				
			ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, led_stripe.led_channel[i]));
		}
		else {
			ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, 
										  led_stripe.led_channel[i], 
										  0));
				
			ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, led_stripe.led_channel[i]));
		}
	}
}

void increase_duty() {
	uint8_t duty[LEDC_SIZE];
	for(uint8_t i = 0; i < LEDC_SIZE; ++i) {
		if(led_stripe.led_on[i]) {
			duty[i] = (led_stripe.led_duty[i] < 100 ? led_stripe.led_duty[i] + 10 : 100);
		}
		else {
			duty[i] = led_stripe.led_duty[i];
		}
	}
	
	set_led_duty(duty);
}

void decrease_duty() {
	uint8_t duty[LEDC_SIZE];
	for(uint8_t i = 0; i < LEDC_SIZE; ++i) {
		if(led_stripe.led_on[i]) {
			duty[i] = (led_stripe.led_duty[i] > 0 ? led_stripe.led_duty[i] - 10 : 0);
		}
		else {
			duty[i] = led_stripe.led_duty[i];
		}
	}
	
	set_led_duty(duty);
}

led_stripe_t led_stripe = {
	.led_pin = {LEDC_RED_GPIO, LEDC_GREEN_GPIO, LEDC_BLUE_GPIO},
	.led_channel = {LEDC_RED_CHANNEL, LEDC_GREEN_CHANNEL, LEDC_BLUE_CHANNEL},
	.led_duty = {0, 0, 0},
	.led_on = {false, false, false}	
};

