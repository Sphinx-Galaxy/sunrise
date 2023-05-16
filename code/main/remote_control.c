/*
 * Author: Mattis Jaksch
 * Date: 20.01.2023
 *
 * Explanation see remote_control.h
 *
 */
 
 #include "remote_control.h"
 
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "ir_tools.h"

static const char* TAG = "IR_REMOTE";

static rmt_channel_t example_rx_channel = 4;

/**
 * @brief RMT Receive Task
 *
 */

static uint32_t receive_remote_control(RingbufHandle_t* rb, ir_parser_t* ir_parser) {
	static bool repeat = false;
	static uint32_t addr = 0;
	static uint32_t cmd = 0;
	static size_t length = 0;
	static rmt_item32_t* items = NULL;
	
	items = (rmt_item32_t*) xRingbufferReceive(rb, &length, portMAX_DELAY);
	if (items) {
		length /= 4; // one RMT = 4 Bytes
		if (ir_parser->input(ir_parser, items, length) == ESP_OK) {
			if (ir_parser->get_scan_code(ir_parser, &addr, &cmd, &repeat) == ESP_OK) {
				ESP_LOGI(TAG, "Scan Code %s --- addr: 0x%04x cmd: 0x%04x", repeat ? "(repeat)" : "", addr, cmd);
			}
		}
		// After parsing the data, return spaces to ringbuffer.
		vRingbufferReturnItem(rb, (void *) items);
	}
	
	// Ignore every cmd that is not for us
	return (addr == IR_REMOTE_ADDR ? cmd : 0);
}

static void decode_remote_control(uint32_t cmd) {

	switch(cmd) {
		case 0xfc03:
			set_led_on((bool [3]){led_stripe.led_duty[0], led_stripe.led_duty[1], led_stripe.led_duty[2]});
			break;
		case 0xfd02:
			set_led_on((bool [3]){false, false, false});
			break;
		case 0xff00:
			increase_duty();
			break;
		case 0xfe01:
			decrease_duty();
			break;
			
		/* White column */
		case 0xf807:
			set_led_on((bool [3]){true, true, true});
			set_led_duty((uint8_t [3]){100, 100, 100});
			break;
			
		/* Red column */
		case 0xfb04:
			set_led_on((bool [3]){true, false, false});
			set_led_duty((uint8_t [3]){100, 0, 0});
			break;
		case 0xf708:
			set_led_on((bool [3]){true, true, false});
			set_led_duty((uint8_t [3]){100, 10, 0});
			break;		
		case 0xf30c:
			set_led_on((bool [3]){true, true, false});
			set_led_duty((uint8_t [3]){100, 20, 0});
			break;				
		case 0xef10:
			set_led_on((bool [3]){true, true, false});
			set_led_duty((uint8_t [3]){100, 30, 0});
			break;		
		case 0xeb14:
			set_led_on((bool [3]){true, true, false});
			set_led_duty((uint8_t [3]){100, 50, 0});
			break;			
			
		/* Green column */
		case 0xfa05:
			set_led_on((bool [3]){false, true, false});
			set_led_duty((uint8_t [3]){0, 100, 0});
			break;		
		case 0xf609:
			set_led_on((bool [3]){false, true, true});
			set_led_duty((uint8_t [3]){0, 100, 20});
			break;		
		case 0xf20d:
			set_led_on((bool [3]){false, true, true});
			set_led_duty((uint8_t [3]){0, 100, 40});
			break;		
		case 0xee11:
			set_led_on((bool [3]){false, true, true});
			set_led_duty((uint8_t [3]){0, 100, 60});
			break;		
		case 0xea15:
			set_led_on((bool [3]){false, true, true});
			set_led_duty((uint8_t [3]){0, 100, 100});
			break;
		
		/* Blue column */
		case 0xf906:
			set_led_on((bool [3]){false, false, true});
			set_led_duty((uint8_t [3]){0, 0, 100});
			break;		
		case 0xf50a:
			set_led_on((bool [3]){true, false, true});
			set_led_duty((uint8_t [3]){20, 0, 100});
			break;		
		case 0xf10e:
			set_led_on((bool [3]){true, false, true});
			set_led_duty((uint8_t [3]){40, 0, 100});
			break;		
		case 0xed12:
			set_led_on((bool [3]){true, false, true});
			set_led_duty((uint8_t [3]){60, 0, 100});
			break;		
		case 0xe916:
			set_led_on((bool [3]){true, false, true});
			set_led_duty((uint8_t [3]){100, 0, 100});
			break;
	}
	
	ESP_LOGI(TAG, "RGB [%d, %d, %d]", led_stripe.led_duty[0], led_stripe.led_duty[1], led_stripe.led_duty[2]);
}


static void handle_remote_control(void *arg) {
	RingbufHandle_t rb = NULL;

	rmt_config_t rmt_rx_config = RMT_DEFAULT_CONFIG_RX(IR_REMOTE_GPIO, example_rx_channel);
	rmt_config(&rmt_rx_config);
	rmt_driver_install(example_rx_channel, 1000, 0);
	ir_parser_config_t ir_parser_config = IR_PARSER_DEFAULT_CONFIG((ir_dev_t)example_rx_channel);
	ir_parser_config.flags |= IR_TOOLS_FLAGS_PROTO_EXT; // Using extended IR protocols (both NEC and RC5 have extended version)
	ir_parser_t* ir_parser = NULL;
	ir_parser = ir_parser_rmt_new_nec(&ir_parser_config);

	//get RMT RX ringbuffer
	rmt_get_ringbuf_handle(example_rx_channel, &rb);
	assert(rb != NULL);
	// Start receive
	rmt_rx_start(example_rx_channel, true);

	while (1) {
		uint32_t cmd = receive_remote_control(rb, ir_parser);
		
		if(cmd != 0) {
			decode_remote_control(cmd);
		}
	}
	
	ir_parser->del(ir_parser);
	rmt_driver_uninstall(example_rx_channel);
	vTaskDelete(NULL);
}

void init_remote_control() {
	xTaskCreate(handle_remote_control, "ir_rx_task", 2048, NULL, 10, NULL);
}
