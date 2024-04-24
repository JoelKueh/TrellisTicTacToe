
#include "mole_whacker.h"

#include <chrono>
#include <cstdlib>
#include <sstream>

#define NOMOLE 16

mole_whacker::mole_whacker(blue_trellis *bt) {
	this->bt = bt;
	game_score=0;
	end=false;

	start_time = new high_resolution_clock::time_point(
			high_resolution_clock::now()
	);
	last_time = new high_resolution_clock::time_point(
			high_resolution_clock::now()
	);

	srand(system_clock::now().time_since_epoch().count());

	//set the scene, probably start with all white LEDS
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

std::string mole_whacker::num_to_string(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

void mole_whacker::update_lcd(std::string input) {
	uint8_t lcd_buff[2][8] = {
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};

	int r=0;
	for (int p = 0; p < 2; p++) {
		for(int q = 0; q < 8; q++) {
			lcd_buff[p][q] = input.at(r);
			r++;

			if (input.length() == r) {
				bt->send_set_lcd(lcd_buff);
				return;
			}
		}
    }
}

//need some sort of polling to call the set_rand_mole every 0.75 s
//when a new mole is selected the colors of the previous mole should be changed to blank
//make it so that after however much time it stops generating random moles, giving user enough time to read their score
//auto end after however much time

void mole_whacker::set_rand_mole() {
	//clear the previous mole
	bt->send_set_led(selected_mole, 0x80, 0x80, 0x80);

	//select a new mole
	selected_mole = rand() & 0xF;

	/*generate random number, n, modulo n by 16, shift the the selected_mole left by n bits to set a random mole
	set the random mole to a different color so that the user knows which one to hit*/
	bt->send_set_led(selected_mole, 0x00, 0x80, 0x80);
}

void mole_whacker::handle_button_press(union blue_trellis::button_event press) {
	//ignore button releases
	if (press.is_rising) {
		return;
	}

	if(press.button_num==selected_mole) {
		//if button pressed==selected_mole, then
		//add points to game_score
		game_score += 10;
		//correct the color of the random mole 
		bt->send_set_led(selected_mole, 0x80, 0x80, 0x80);
		selected_mole=NOMOLE;
	}

	/*display score on LCD display whether button press was success or not*/
	std::string lcd_output=num_to_string(game_score);
	update_lcd(lcd_output);

}

void mole_whacker::update() {
	// Get the time between this frame and the last
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - *last_time);
	auto total_time = duration_cast<seconds>(now - *start_time);

	//if the game has been running for less than 20 seconds
	if(total_time.count() < 20) {
		// If 750 milliseconds have passed, update the time.
		if (duration.count() > 750) {
			set_rand_mole();

			// Update our last frame time
			delete last_time;
			last_time = new high_resolution_clock::time_point(now);
		}
	}

	else if (total_time.count() > 30) {
		end=true;
	}
	
	//get a button event
	union blue_trellis::button_event event;
	char header = bt->poll_header();
	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event=bt->get_button_event();
		handle_button_press(event);	
	}
}

bool mole_whacker::is_done() {
	return end;
}

mole_whacker::~mole_whacker() {
	delete start_time;
	delete last_time;

}
