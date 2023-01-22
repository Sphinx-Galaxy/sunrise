/*
 * Author: Mattis Jaksch
 * Date: 18.01.2023
 *
 * Explanation
 *
 */
 
#pragma once

#define CONFIG_SNTP_TIME_SERVER "pool.ntp.org"

// TODO Make this time time variable maybe (?)
/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
 
//RTC_DATA_ATTR static int boot_count = 0;

void obtain_time();
void initialize_sntp();


void test_alarm();

 // Timer construct
 
 // Time light correlation array
 
 // Get absolute time
 
 // Set alarm time
