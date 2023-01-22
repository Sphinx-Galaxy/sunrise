/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation see led_stripe.h
 *
 */
 
#include "led_stripe.h"

#include "esp_err.h"

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

void set_red_led_duty(uint8_t duty) {
	ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_RED_CHANNEL, 8191.0 * duty / 100.0));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_RED_CHANNEL));
}

void set_green_led_duty(uint8_t duty) {
	ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_GREEN_CHANNEL, 8191.0 * duty / 100.0));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_GREEN_CHANNEL));
}

void set_blue_led_duty(uint8_t duty) {
	ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_BLUE_CHANNEL, 8191.0 * duty / 100.0));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_BLUE_CHANNEL));
}
