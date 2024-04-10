
#include <iostream>
#include <string>

#include "blue_trellis.h"

const std::string bluetooth_addr = "00:14:03:05:5E:92";

int main()
{
	blue_trellis bt = blue_trellis(bluetooth_addr);
	bt.send_set_led(0, 0xFF, 0x00, 0x00);
	while (1);
}
