
#include <blue_trellis.h>

#include <iostream>
#include <iomanip>
#include <sstream>

blue_trellis::blue_trellis(std::string addr)
{
	this->addr = addr;
	port = BTSerialPortBinding::Create(addr, 1);
	port->Connect();
}

void blue_trellis::send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b)
{
	char buffer[] = { SET_LED_HEADER, (char)num, (char)g, (char)r, (char)b };
	port->Write(buffer, 5);
}

void blue_trellis::send_set_leds(uint8_t colors[16][3])
{

}

void blue_trellis::send_set_lcd(uint8_t data[2][8])
{

}

void blue_trellis::print_data()
{
	std::stringstream str;
	std::string output;
	unsigned char buffer[30];
	int bytes = 0;
	if ((bytes = port->Read((char *)buffer, 30)) == 0)
		return;

	for (int i = 0; i < bytes; ++i) {
		if (std::isalpha(buffer[i])) {
			str << buffer[i];
		} else {
			str << "0x" << std::setw(2) << std::setfill('0')
				<< std::hex << (uint32_t)buffer[i];
		}
		str << ", ";
	}
	output = str.str();
	output.pop_back();
	output.pop_back();
	std::cout << output << std::flush;
}

blue_trellis::~blue_trellis()
{
	port->Close();
	delete port;
}
