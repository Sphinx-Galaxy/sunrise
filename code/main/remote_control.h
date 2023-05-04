/*
 * Author: Mattis Jaksch
 * Date: 20.01.2023
 *
 * Explanation
 *
 */
 
#pragma once

#include "led_stripe.h"

#define IR_REMOTE_GPIO (15)
#define IR_REMOTE_ADDR (0xef00)

void init_remote_control();
