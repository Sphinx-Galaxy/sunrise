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
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

static const char* TAG = "MAIN";

 // Interrupt for LED remote controller
 
 // Interrupt for time
 
 // Set via WIFI
 
 void sunrise() {
	//TODO Stop this task if the remote has been used
	
 	// Duty Cycle: 
 	// 1, 2, 3, 4, 5, 6, 7, 9, 10
 	// 10, 20, 30, 40, 50, 60, 70, 80, 90, 100
 	
 	uint8_t scale = 10;
 	
 	// Reset the stripe
	set_led_on((bool [3]){false, false, false});
 	set_led_duty((uint8_t [3]){0, 0, 0});
	set_led_on((bool [3]){true, true, true});
	
 	for(uint8_t i = 1; i < 101; i++) {
 		set_led_duty((uint8_t [3]){i, 0, 0});
	    vTaskDelay(scale * 100 / portTICK_PERIOD_MS);
 	}
 	
 	for(uint8_t i = 1; i < 11; i++) {
 		set_led_duty((uint8_t [3]){100, i, 0});
	    vTaskDelay(scale * 10 * 100 / portTICK_PERIOD_MS);
 	}
 	
 	for(uint8_t i = 1; i < 101; i++) {
 		if(i < 10) {
	 		set_led_duty((uint8_t [3]){100, (i + 10), i}); 		
 		}
 		else {
	 		set_led_duty((uint8_t [3]){100, (i < 50 ? i * 2 : 100), i}); 
 		}
 		
	    vTaskDelay(scale * 100 / portTICK_PERIOD_MS);
 	}
 }
  
void app_main(void) {
	/* Setup LED stripe and remote control */
	init_led_stripe();
	init_remote_control();
	
	/* Init the wifi and to get the time */
	init_wifi();
	init_alarm();
	
	/* Set timer */
	time_t now = 0;
    struct tm timeinfo = { 0 };
    
    time(&now);
    localtime_r(&now, &timeinfo);
    
    if(timeinfo.tm_hour > 6) {
    	now += 24*60*60;
   		localtime_r(&now, &timeinfo);
    }
    
    timeinfo.tm_hour = 5;
    timeinfo.tm_min = 55;
    timeinfo.tm_sec = 0;
    time_t alarm_time = mktime(&timeinfo);
		
	/* Wait for timer */
	while(true) {
		time(&now);
		ESP_LOGI(TAG, "Time Now: %d; Alarm at: %d; Remaining: %d", (int)now, (int)alarm_time, (int)(alarm_time-now));	
		
		if(now > alarm_time) { // Check for 6 Uhr
			sunrise();    
			
			/* Set next alarm */
			time(&now);			
			now += 24*60*60;
			localtime_r(&now, &timeinfo);
			
			timeinfo.tm_hour = 5;
			timeinfo.tm_min = 55;
			timeinfo.tm_sec = 0;
			alarm_time = mktime(&timeinfo);    
		}
		vTaskDelay(60*1000 / portTICK_PERIOD_MS);
		init_alarm();
	}
 }
