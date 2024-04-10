
#include <iostream>
#include <string>

#include "blue_trellis.h"

const std::string bluetooth_addr = "00:14:03:05:5E:92";

int main()
{
	std::cout << "Connecting..." << std::endl;
	blue_trellis bt = blue_trellis(bluetooth_addr);
	std::cout << "Connected" << std::endl;
	std::cout << "Sending Command...\nResponse: " << std::flush;
	bt.send_set_led(0, 0xFF, 0x00, 0x00);

	while (true) {
		long i = 1000000000;
		while (i--);
		bt.print_data();
	}
	
	return 0;
}
