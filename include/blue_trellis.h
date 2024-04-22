
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
    
	/**
	 * A struct that defines the layout of a button_event
	 */
	union button_event {
		struct {
			uint8_t button_num : 7;
			uint8_t is_rising : 1;
		};
		uint8_t raw;
	};

	/**
	 * A struct that defines the layout of a set_led command
	 */
	union set_led {
		struct {
			uint8_t led_num;
			uint8_t color[3]; // RGB values for the led.
		};
		uint8_t raw;
	};

	/**
	 * A struct that defines the layout of a set_display command
	 */
	union set_display {
		struct {
			uint8_t colors[16][3]; // Array of 16 GRB values.
		};
		uint8_t raw;
	};

	/**
	 * A struct that defines the layout of a set_lcd comamnd
	 */
	union set_lcd {
		struct {
			uint8_t data[2][8]; // Array of strings for top and bottom row.
		};
		uint8_t raw;
	};

	/**
	 * Constructs a blue_trellis object
	 * @param addr a string containing the address of the bluetooth device
	 */
	blue_trellis(std::string addr);

	/**
	 * Sends a set_led command over bluetooth.
	 * @param num The number of the led to set on the button pad
	 * @param g The green value
	 * @param r The red value
	 * @param b The blue value
	 */
	void send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);

	/**
	 * Sends a set_display command over bluetooth
	 * @param colors An array of 16 GRB encoded colors to be displayed
	 */
	void send_set_display(const uint8_t colors[16][3]);

	/**
	 * Sends a set_lcd command over bluetooth
	 * @param data The array of characters to be written to the lcd
	 */
	void send_set_lcd(const uint8_t data[2][8]);

	/**
	 * Non-blocking function that polls for a command header.
	 * @return The header recieved (otherwise NULL)
	 */
	char poll_header();

	/**
	 * Performs a blocking wait for the body of a button_event
	 * @return A struct containing the button event data.
	 */
	union button_event get_button_event();

	/**
	 * Destroys a blue_trellis object
	 */
	~blue_trellis();

	std::string addr;
private:
	/**
	 * Blocking function that waits for the rest of the function body.
	 * @param dest Where to put the data
	 * @param bytes The number of bytes to get
	 */
	void get_body(uint8_t *dest, int bytes);

	/**
	 * Entrypoint for the input-handling thread.
	 * Takes data from the bluetooth functions and puts it in a buffer.
	 */
	void loop_poll();

#ifdef BLUE_STDIO
	/**
	 * Used for debugging over STDIO
	 * Prints data over the 
	 */
	void print_trellis();
#endif

	std::thread *poller;
	std::deque<uint8_t> rx_buffer;
	uint8_t colors[16][3];

	BTSerialPortBinding *port;
};

#endif /* BLUE_TRELLIS_H */
