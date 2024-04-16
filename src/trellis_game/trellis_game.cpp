
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "blue_trellis.h"

const std::string bluetooth_addr = "00:14:03:05:5E:92";

int main()
{
#ifndef BLUE_STDIO
	std::cout << "Connecting..." << std::endl;
#endif
	blue_trellis bt = blue_trellis(bluetooth_addr);
#ifndef BLUE_STDIO
	std::cout << "Connected" << std::endl;
	std::cout << "Sending Command...\nResponse: " << std::flush;
#endif

	uint8_t colors[16][3] = {
		{ 0xF0, 0x00, 0x00 },
		{ 0xE0, 0x10, 0x00 },
		{ 0xD0, 0x20, 0x00 },
		{ 0xC0, 0x30, 0x00 },
		{ 0xB0, 0x40, 0x00 },
		{ 0xA0, 0x50, 0x00 },
		{ 0x90, 0x60, 0x00 },
		{ 0x80, 0x70, 0x00 },
		{ 0x70, 0x80, 0x00 },
		{ 0x60, 0x90, 0x00 },
		{ 0x50, 0xA0, 0x00 },
		{ 0x40, 0xB0, 0x00 },
		{ 0x30, 0xC0, 0x00 },
		{ 0x20, 0xD0, 0x00 },
		{ 0x10, 0xE0, 0x00 },
		{ 0x00, 0xF0, 0x00 }
	};

	uint8_t lcd[2][8] = {
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08 },
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08 }
	};

	bt.send_set_display(colors);
	bt.send_set_lcd(lcd);

	while (true) {
		char header = bt.poll_header();
		union blue_trellis::button_event event;
		if (header == blue_trellis::BUTTON_EVENT_HEADER) {
			event = bt.get_button_event();
			std::cout << "Button " << (int)event.button_num
				<< " was "
				<< (event.is_rising ? "pressed" : "released")
				<< std::endl;
		}
	}
	
	return 0;
}
