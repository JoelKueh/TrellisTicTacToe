
#ifndef BLUE_TRELLIS_H
#define BLUE_TRELLIS_H

#include <string>

#include "bsp/BTSerialPortBinding.h"

#include <deque>
#include <thread>

class blue_trellis {
public:
	enum {
		BUTTON_EVENT_HEADER = 'A',
		SHOW_HEADER = 'C',
		SET_LED_HEADER = 'D',
		SET_DISPLAY_HEADER = 'E',
		SET_LCD_HEADER = 'F',
	};
    
	union button_event {
		struct {
			uint8_t button_num : 7;
			uint8_t is_rising : 1;
		};
		uint8_t raw;
	};

	union set_led {
		struct {
			uint8_t led_num;
			uint8_t color[3]; // RGB values for the led.
		};
		uint8_t raw;
	};

	union set_display {
		struct {
			uint8_t colors[16][3]; // Array of 16 GRB values.
		};
		uint8_t raw;
	};

	union set_lcd {
		struct {
			uint8_t data[2][8]; // Array of strings for top and bottom row.
		};
		uint8_t raw;
	};

	blue_trellis(std::string addr);

	void send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);
	void send_set_display(const uint8_t colors[16][3]);
	void send_set_lcd(const uint8_t data[2][8]);

	/**
	 * Non-blocking function that polls for a command header.
	 */
	char poll_header();
	/**
	 * Performs a blocking wait for the body of a button_event
	 */
	union button_event get_button_event();

	void print_data();

	~blue_trellis();

	std::string addr;
private:
	/**
	 * Blocking function that waits for the rest of the function body.
	 */
	void get_body(uint8_t *dest, int bytes);

	void loop_poll();

#ifdef BLUE_STDIO
	void print_trellis();
#endif

	std::thread *poller;
	std::deque<uint8_t> rx_buffer;
	uint8_t colors[16][3];

	BTSerialPortBinding *port;
};

#endif /* BLUE_TRELLIS_H */
