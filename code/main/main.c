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
 
 // Interrupt for LED remote controller
 
 // Interrupt for time
 
 // Set via WIFI
 
struct test_t {
	int var;
	void (*aah)(void);
} test = {0};
 
void app_main(void) {
	init_led_stripe();
	
	led_stripe.set_red(0);
 
 	//test.aah();
 
 
 	// Initialize stuff
 
 	// Go to sleep
 }
