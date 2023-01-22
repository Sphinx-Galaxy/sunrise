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
 	// Duty Cycle: 
 	// 1, 2, 3, 4, 5, 6, 7, 9, 10
 	// 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
 	
 	for(uint8_t i = 1; i < 10; i++) {
 		set_red_led_duty(i); // 1 .. 9
 		
 		if(i >= 5) {
 			set_green_led_duty(i - 4); // 1 .. 5
 		}
 		
	    vTaskDelay(60*1000 / portTICK_PERIOD_MS);
 	}
 	
 	for(uint8_t i = 1; i < 11; i++) {
 		set_red_led_duty(i * 10); // 10 .. 100
 		
 		if(i < 5) {
	 		set_green_led_duty(i + 5); // 6  ... 9
	 	}
	 	else {
	 		set_green_led_duty((i - 5) * 10); // 10  ... 50	 	
	 	}
	 	
 		set_blue_led_duty(i); // 1 ... 10
 		
	    vTaskDelay(60*1000 / portTICK_PERIOD_MS);
 	}
 }
  
void app_main(void) {
 	//init_led_stripe();
 	
	//sunrise(); // Move this to an interrupt with time

	test_wifi();
	
	test_alarm();


/*	init_led_stripe();*/
/*	*/
/*	for(int i = 0; i < 10; ++i) {*/
/*		set_red_led_duty(i * 10);*/
/*	}*/
		
	
 	// Initialize stuff
 
 	// Go to sleep
 }
