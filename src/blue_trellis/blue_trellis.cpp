
#include "blue_trellis.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef BLUE_STDIO

#include <deque>
std::deque<uint8_t> stdio_buffer;

void loop_poll_stdin()
{
	while (true) {
		char c;
		std::cin >> c;
		stdio_buffer.push_back(c);
	}
}

#endif

blue_trellis::blue_trellis(std::string addr)
{
#ifdef BLUE_STDIO
	poller = new std::thread(loop_poll_stdin);
#else
	std::cout << "Connecting..." << std::endl;
	this->addr = addr;
	port = BTSerialPortBinding::Create(addr, 1);
	port->Connect();
	std::cout << "Connected" << std::endl;
	std::cout << "Sending Command...\nResponse: " << std::flush;
#endif
}

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
	static char header = SET_DISPLAY_HEADER;
	port->Write(&header, 1);
#endif
}

char blue_trellis::poll_header()
{
	char header = 0;
#ifdef BLUE_STDIO
	if (!stdio_buffer.empty()) {
		header = stdio_buffer.front();
		stdio_buffer.pop_front();
	}
#else
	port->Read(&header, 1);
#endif
	return header;
}

void blue_trellis::get_body(uint8_t *dest, int bytes)
{
#ifdef BLUE_STDIO
	while (bytes--) {
		while (stdio_buffer.empty());
		*dest++ = stdio_buffer.front();
		stdio_buffer.pop_front();
	}
#else
	while (bytes) {
		int read = port->Read((char *)dest, bytes);
		bytes -= read;
	}
#endif
}

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

void blue_trellis::print_data()
{
#ifdef BLUE_STDIO

#else
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
#endif
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

blue_trellis::~blue_trellis()
{
#ifdef BLUE_STDIO
	delete poller;
#else
	port->Close();
	delete port;
#endif
}
