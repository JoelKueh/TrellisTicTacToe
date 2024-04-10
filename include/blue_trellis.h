
#include <string>

#include "bsp/BTSerialPortBinding.h"

class blue_trellis {
public:
	enum {
		BUTTON_EVENT_HEADER = 'A',
		SET_LED_HEADER = 'D',
		SET_LEDS_HEADER = 'E',
		SET_LCD_HEADER = 'F',
	};
    
	struct button_event {
		uint8_t button_num : 7;
		uint8_t is_rising : 1;
	};

	struct set_led {
		uint8_t led_num;
		uint8_t color[3]; // RGB values for the led.
	};

	struct set_leds {
		uint8_t colors[16][3]; // Array of 16 GRB values.
	};

	struct set_lcd {
		uint8_t data[2][8]; // Array of strings for top and bottom row.
	};

	blue_trellis(std::string addr);

	void send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b);
	void send_set_leds(uint8_t colors[16][3]);
	void send_set_lcd(uint8_t data[2][8]);

	void print_data();

	~blue_trellis();

	std::string addr;
private:
	BTSerialPortBinding *port;
};

