
#include "calculator.h"
#include <sstream>

#define INVALIDNUM 100

//enter calculator mode
calculator::calculator(blue_trellis *bt) {
	this->bt = bt;
	//left 3 columns be blue to represent 0-9, exit, and =; and right column be red to represent +, -, *, /
	const uint8_t display_leds [16][3] = {
		{0x00, 0x00, 0x80}, //blue, button 0
		{0x00, 0x00, 0x80}, //blue, button 1
		{0x00, 0x00, 0x80}, //blue, button 2
		{0x00, 0x80, 0x00}, //red, button 3
		{0x00, 0x00, 0x80}, //blue, button 4
		{0x00, 0x00, 0x80}, //blue, button 5
		{0x00, 0x00, 0x80}, //blue, button 6
		{0x00, 0x80, 0x00}, //red, button 7
		{0x00, 0x00, 0x80}, //blue, button 8
		{0x00, 0x00, 0x80}, //blue, button 9
		{0x00, 0x00, 0x80}, //blue, button 10
		{0x00, 0x80, 0x00}, //red, button 11
		{0x00, 0x80, 0x80}, //purple, button 12
		{0x00, 0x00, 0x80}, //blue, button 13
		{0x80, 0x00, 0x00}, //green, button 14
		{0x00, 0x80, 0x00} //red, button 15
	};

	//initiliaze all variables to be empty or default case
	num_buff[0] = 0;
	num_buff[1] = 0;
	end = 0;
	is_second_num = 0;
	last_op = ' ';

	//sets display with all values set in display_leds
	bt->send_set_display(display_leds);
}

//convert an int to a string
std::string calculator::num_to_string(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

//convert a float to a string
std::string calculator::num_to_string(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

void calculator::update_lcd(std::string input) {
	//clear all characters out of LCD buffer
	uint8_t lcd_buff[2][8] = {
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};

	int char_num=0;

	for (int p = 0; p < 2; p++) {
		for(int q = 0; q < 8; q++) {
			lcd_buff[p][q] = input.at(char_num);
			char_num++;

			//when at end of string, display lcd_buff
			if (input.length() == char_num) {
				bt->send_set_lcd(lcd_buff);
				return;
			}
		}
    }

	//if end of string not reached, then send blank display
	bt->send_set_lcd(lcd_buff);
}

//use selected operation to compute total
int calculator::calc_total(float num1, float num2, char op) {
	if(op=='+') {
		return num1 + num2;
	}
	else if (op=='-') {
		return num1-num2;
	}
	else if (op=='*') {
		return num1 * num2;
	}
	else if (op=='/') {
		if (num2 == 0) {
			return 0;
		}
		return num1 / num2;
	}
	return 0;
}

void calculator::handle_button_event(union blue_trellis::button_event press) {
	//only count button presses, not releases
	if (press.is_rising) {
		return;
	}
	
	//initiliaze variables to default values
	int input_num = INVALIDNUM;
	int input_op = ' ';
	
	switch(press.button_num) {
		case 0:
			input_num=1;
			break;
		case 1:
			input_num=2;
			break;
		case 2:
			input_num=3;
			break;
		//addition
		case 3:
			input_op='+';
			break;
		case 4:
			input_num=4;
			break;
		case 5:
			input_num=5;
			break;
		case 6:
			input_num=6;
			break;
		//subtraction
		case 7:
			input_op='-';
			break;
		case 8:
			input_num=7;
			break;
		case 9:
			input_num=8;
			break;
		case 10:
			input_num=9;
			break;
		//multiplication
		case 11:
			input_op='*';
			break;
		//exit
		case 12:
			input_op='e';
			break;
		case 13:
			input_num=0;
			break;
		//equals
		case 14:
			input_op='=';
			break;
		//division
		case 15:
			input_op='/';
			break;
	}

	//if a number button was pressed update value
	if(input_num != INVALIDNUM) {
		num_buff[is_second_num] *= 10;
		num_buff[is_second_num] += input_num;

		//convert int to string and then hand off to update_lcd to display
		std::string numStr = num_to_string(num_buff[is_second_num]);
		update_lcd(numStr);
	}
	else if(input_op=='e') {
		end=true;
	}
	else if(input_op=='=') {
		//update total
		num_buff[0] = calc_total(num_buff[0], num_buff[1], last_op);
		//convert int to string and then hand off to update_lcd
		std::string numStr= num_to_string(num_buff[0]);
		update_lcd(numStr);

		//set variables back to default values
		is_second_num = false;
		num_buff[0] = 0;
		num_buff[1] = 0;
		last_op = ' ';
	}

	//if an input_op button was pressed that isn't equals or escape
	else if(input_op != ' ') {
		//don't use operation if only one number has been sent
		if (!is_second_num) {
			is_second_num = true;
			last_op = input_op;
			return;
		}

		//update total
		num_buff[0] = calc_total(num_buff[0], num_buff[1], last_op);
		//convert int to string and then hand off to update_lcd
		std::string numStr= num_to_string(num_buff[0]);
		update_lcd(numStr);
		
		//shift buffer and last operation one to the left
		num_buff[1] = 0;
		last_op = input_op;
	}
}

void calculator::update() {
	//create a variable of type button_event
	union blue_trellis::button_event event;
	char header = bt->poll_header();
	
	//if a button event has happened then call handle_button_event
	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event=bt->get_button_event();
		handle_button_event(event);	
	}
}

bool calculator::is_done() {
	return end;
}

//nothing to destroy in this program
calculator::~calculator() {


}
