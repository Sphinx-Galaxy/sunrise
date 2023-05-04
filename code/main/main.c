/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation
 *
 */
 
 #include "alarm_clock.h"
 #include "led_stripe.h"
 #include "remote_control.h"
 
 #include "wifi_control.h"
 
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

 // Interrupt for LED remote controller
 
 // Interrupt for time
 
 // Set via WIFI
 
 void sunrise() {
	//TODO Stop this task if the remote has been used
	
 	// Duty Cycle: 
 	// 1, 2, 3, 4, 5, 6, 7, 9, 10
 	// 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
 	
 	// Reset the stripe
	set_led_on((bool [3]){false, false, false});
 	set_led_duty((uint8_t [3]){0, 0, 0});
	set_led_on((bool [3]){true, true, true});
	
 	for(uint8_t i = 1; i < 10; i++) {
 		set_led_duty((uint8_t [3]){i, (i >= 5 ? i-4 : 0), 0});
	    vTaskDelay(1000 / portTICK_PERIOD_MS);
 	}
 	
 	for(uint8_t i = 1; i < 11; i++) { 		
 		set_led_duty((uint8_t [3]){i*10, (i <= 5 ? i + 5 : (i - 5) * 10), i});
	    vTaskDelay(1000 / portTICK_PERIOD_MS);
 	}
 }
  
void app_main(void) {
	init_led_stripe();
	init_remote_control();
	
	// Init the wifi and to get the time
	// test_wifi()
	while(true) {
		if(true) { // Check for 6 Uhr
			sunrise();
		}
		// Sleep
	}
 }
