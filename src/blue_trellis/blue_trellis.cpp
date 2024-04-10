
#include <blue_trellis.h>

blue_trellis::blue_trellis(std::string addr)
{
	this->addr = addr;
	port = BTSerialPortBinding::Create(addr, 1);
	port->Connect();
}

void blue_trellis::send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b)
{
	char buffer[] = { SET_LED_HEADER, (char)num, (char)g, (char)r, (char)b };
	port->Write(buffer, 4);
}

void blue_trellis::send_set_leds(uint8_t colors[16][3])
{

}

void blue_trellis::send_set_lcd(uint8_t data[2][8])
{

}

blue_trellis::~blue_trellis()
{
	port->Close();
	delete port;
}
