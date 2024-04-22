
#include "blue_trellis.h"

#include <iostream>
#include <iomanip>
#include <sstream>

/**
 * Constructs a blue_trellis object
 * @param addr a string containing the address of the bluetooth device
 */
blue_trellis::blue_trellis(std::string addr)
{
#ifndef BLUE_STDIO
	std::cout << "Connecting..." << std::endl;
	this->addr = addr;
	port = BTSerialPortBinding::Create(addr, 1);
	port->Connect();

	std::cout << "Connected" << std::endl;
	std::cout << "Sending Command...\nResponse: " << std::flush;
#endif
	// Start a thread that runs the supplied lambda and polls the input.
	poller = new std::thread([this]() { loop_poll(); });
}

/**
 * Entrypoint for the input-handling thread.
 * Takes data from the bluetooth functions and puts it in a buffer.
 */
void blue_trellis::loop_poll()
{
	while (true) {
		char c;
#ifdef BLUE_STDIO
		std::cin >> c;
#else
		port->Read(&c, 1);
#endif
		rx_buffer.push_back(c);
	}
}

/**
 * Sends a set_led command over bluetooth.
 * @param num The number of the led to set on the button pad
 * @param g The green value
 * @param r The red value
 * @param b The blue value
 */
void blue_trellis::send_set_led(uint8_t num, uint8_t g, uint8_t r, uint8_t b)
{
#ifdef BLUE_STDIO
	colors[num][0] = g;
	colors[num][1] = r;
	colors[num][2] = b;
	print_trellis();
#else
	char buffer[] = { SET_LED_HEADER, (char)num, (char)g, (char)r, (char)b };
	port->Write(buffer, 5);
#endif
}

/**
 * Sends a set_display command over bluetooth
 * @param colors An array of 16 GRB encoded colors to be displayed
 */
void blue_trellis::send_set_display(const uint8_t colors[16][3])
{
#ifdef BLUE_STDIO
	for (int i = 0; i < 16; ++i) {
		this->colors[i][0] = colors[i][0];
		this->colors[i][1] = colors[i][1];
		this->colors[i][2] = colors[i][2];
	}
	print_trellis();
#else
	static char header = SET_DISPLAY_HEADER;
	port->Write(&header, 1);
	port->Write((char *)colors, 16 * 3);
#endif
}

/**
 * Sends a set_lcd command over bluetooth
 * @param data The array of characters to be written to the lcd
 */
void blue_trellis::send_set_lcd(const uint8_t data[2][8])
{
#ifdef BLUE_STDIO
	std::cout << "LCD State" << std::endl
		<< "===============================================" << std::endl;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 8; ++j) {
			std::cout << "0x" << std::setw(2) << std::setfill('0')
				<< std::hex << (uint32_t)data[i][j]
				<< ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "==============================================="
		<< std::endl << std::endl;
#else
	static char header = SET_LCD_HEADER;
	port->Write(&header, 1);
	port->Write((const char *)data, 16);
#endif
}

/**
 * Non-blocking function that polls for a command header.
 * @return The header recieved (otherwise NULL)
 */
char blue_trellis::poll_header()
{
	char header = 0;

	if (!rx_buffer.empty()) {
		header = rx_buffer.front();
		rx_buffer.pop_front();
	}

	return header;
}

/**
 * Blocking function that waits for the rest of the function body.
 * @param dest Where to put the data
 * @param bytes The number of bytes to get
 */
void blue_trellis::get_body(uint8_t *dest, int bytes)
{
	while (bytes--) {
		while (rx_buffer.empty());
		*dest++ = rx_buffer.front();
		rx_buffer.pop_front();
	}
}

/**
 * Performs a blocking wait for the body of a button_event
 * @return A struct containing the button event data.
 */
union blue_trellis::button_event blue_trellis::get_button_event()
{
	union button_event event;
#ifdef BLUE_STDIO
	uint8_t data[2];
	get_body(data, 2);
	data[0] = tolower(data[0]);
	data[1] = tolower(data[1]);

	switch (data[0]) {
		case '0':
			event.button_num = 0;
			break;
		case '1':
			event.button_num = 1;
			break;
		case '2':
			event.button_num = 2;
			break;
		case '3':
			event.button_num = 3;
			break;
		case '4':
			event.button_num = 4;
			break;
		case '5':
			event.button_num = 5;
			break;
		case '6':
			event.button_num = 6;
			break;
		case '7':
			event.button_num = 7;
			break;
		case '8':
			event.button_num = 8;
			break;
		case '9':
			event.button_num = 9;
			break;
		case 'a':
			event.button_num = 10;
			break;
		case 'b':
			event.button_num = 11;
			break;
		case 'c':
			event.button_num = 12;
			break;
		case 'd':
			event.button_num = 13;
			break;
		case 'e':
			event.button_num = 14;
			break;
		case 'f':
			event.button_num = 15;
			break;
	}

	event.is_rising = data[1] == 'p';
#else
	get_body((uint8_t *)&event, 1);
#endif
	return event;
}

#ifdef BLUE_STDIO
void blue_trellis::print_trellis()
{
	static std::string reset = "\033[m";

	std::cout << std::endl << "Button States" << std::endl
		  << "================";

	for (int i = 0; i < 16; ++i) {
		if (i % 4 == 0)
			std::cout << std::endl << std::endl << ' ';

		std::string buffer = "\033[48;2;";
		buffer.append(std::to_string(colors[i][1]));
		buffer.push_back(';');
		buffer.append(std::to_string(colors[i][0]));
		buffer.push_back(';');
		buffer.append(std::to_string(colors[i][2]));
		buffer.push_back('m');
		buffer.push_back(' ');
		buffer.push_back(' ');
		std::cout << buffer;
		std::cout << reset << "  ";
	}

	std::cout << reset << std::endl << std::endl
		  << "================="
		  << std::endl << std::endl;
}
#endif

/**
 * Destroys a blue_trellis object
 */
blue_trellis::~blue_trellis()
{
	delete poller;
	port->Close();
	delete port;
}
