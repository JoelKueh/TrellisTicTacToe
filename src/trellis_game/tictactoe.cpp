
#include "tictactoe.h"

#define PONE 0
#define PTWO 1

tictactoe::tictactoe(blue_trellis *bt) {
	this->bt = bt;

	end=false; 
	player=PONE;

	//set the scene, all white LEDS
	const uint8_t display_leds [16][3] = {
		{0x80, 0x80, 0x80}, //white, button 0
		{0x80, 0x80, 0x80}, //white, button 1
		{0x80, 0x80, 0x80}, //white, button 2
		{0x80, 0x80, 0x80}, //white, button 3
		{0x80, 0x80, 0x80}, //white, button 4
		{0x80, 0x80, 0x80}, //white, button 5
		{0x80, 0x80, 0x80}, //white, button 6
		{0x80, 0x80, 0x80}, //white, button 7
		{0x80, 0x80, 0x80}, //white, button 8
		{0x80, 0x80, 0x80}, //white, button 9
		{0x80, 0x80, 0x80}, //white, button 10
		{0x80, 0x80, 0x80}, //white, button 11
		{0x80, 0x80, 0x80}, //white, button 12
		{0x80, 0x80, 0x80}, //white, button 13
		{0x80, 0x80, 0x80}, //white, button 14
		{0x80, 0x80, 0x80} //white, button 15
	};
	//sets display with all values set in display_leds
	bt->send_set_display(display_leds);
}

void tictactoe::handle_button_event(union blue_trellis::button_event press) {
	//only count button presses, not releases
	if (press.is_rising) {
		return;
	}

	if(player==PONE) {
			bt->send_set_led(press.button_num, 0x30, 0xD5, 0xC8); //yellow
	}
	else {
		bt->send_set_led(press.button_num, 0xD5, 0x30, 0xC8); //pinkish-purple
	}

	player=!player;
	

}

void tictactoe::update() {
	//declares a variable of type button_event
	union blue_trellis::button_event event;
	char header = bt->poll_header();
	
	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event=bt->get_button_event();
		handle_button_event(event);	
	}
}

bool tictactoe::is_done() {
	return end;
}

tictactoe::~tictactoe() {

}
