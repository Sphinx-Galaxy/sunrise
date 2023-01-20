/*
 * Author: Mattis Jaksch
 * Date: 20.01.2023
 *
 * Explanation
 *
 */
 
#pragma once

#include "led_stripe.h"
 
// Remote control struct

// Decoding array

struct led_stripe_settings decode_remote_controller(uint8_t* input, uint8_t size);
