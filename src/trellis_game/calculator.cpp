
#include "calculator.h"
#include <sstream>

int end=0;
uint8_t s[2][8];

int num[100]={0};
int i=0;
int r=0;
float total=0;
int numOperations=0;

char operation[100] = {" "};
//std::string output="0";

//enter calculator mode
calculator::calculator(blue_trellis *bt)
{
	this->bt = bt;
	//left 3 columns be blue to represent 0-11, and right column be red to represent +, -, *, /
	const uint8_t display_leds [16][3] = {
	{0x00, 0x00, 0x80}, //blue, button 0
	{0x00, 0x00, 0x80}, //blue, button 1
	{0x00, 0x00, 0x80}, //blue, button 2
	{0x00, 0x00, 0x80}, //blue, button 3
	{0x00, 0x00, 0x80}, //blue, button 4
	{0x00, 0x00, 0x80}, //blue, button 5
	{0x00, 0x00, 0x80}, //blue, button 6
	{0x00, 0x00, 0x80}, //blue, button 7
	{0x00, 0x00, 0x80}, //blue, button 8
	{0x00, 0x00, 0x80}, //blue, button 9
	{0x00, 0x00, 0x80}, //blue, button 10
	{0x00, 0x00, 0x80}, //blue, button 11
	{0x00, 0x80, 0x00}, //red, button 12
	{0x00, 0x80, 0x00}, //red, button 13
	{0x00, 0x80, 0x00}, //red, button 14
	{0x00, 0x80, 0x00} //red, button 15
	};

	//sets display with all values set in display_leds
	bt->send_set_display(display_leds);
}

std::string calculator::num_to_string(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

std::string calculator::num_to_string(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

void calculator::string_to_arr(std::string input) {
	for (int p = 0; p < 2; p++) {
		for(int q = 0; q < 8; q++) {
			s[p][q]=input.at(r);
			r++;
		}
    }
	r=0;
}

void calculator::calc_total() {
	if(i>0) {
		if(operation[i]=='+') {
			total=num[i-1]+num[i];
			num[i]=total;
		}
		else if (operation[i]=='-') {
			total=num[i-1]-num[i];
			num[i]=total;
		}
		else if (operation[i]=='*') {
			total=num[i-1]*num[i];
			num[i]=total;
		}
		else if (operation[i]=='/') {
			total=num[i-1]/num[i];
			num[i]=total;
		}
	}
	else {
		total=num[i];
	}
}

void calculator::handle_button_event(union blue_trellis::button_event press) {
	//only count button presses, not releases
	if (press.is_rising) {
		return;
	}
	
	int temp=100;
	operation[i]=' ';
	
	//clear output string
	for(int l=0; l<2; l++) {
		for(int m=0; m<8; m++) {
			s[l][m]=' ';
		}
	}
	
	switch(press.button_num) {
		case 0:
			temp=1;
			break;
		case 1:
			temp=2;
			break;
		case 2:
			temp=3;
			break;
		//addition
		case 3:
			operation[i]='+';
			break;
		case 4:
			temp=4;
			break;
		case 5:
			temp=5;
			break;
		case 6:
			temp=6;
			break;
		//subtraction
		case 7:
			operation[i]='-';
			break;
		case 8:
			temp=7;
			break;
		case 9:
			temp=8;
			break;
		case 10:
			temp=9;
			break;
		//multiplication
		case 11:
			operation[i]='*';
			break;
		//escape
		case 12:
			operation[i]='e';
			break;
		case 13:
			temp=0;
			break;
		//equals
		case 14:
			operation[i]='=';
			break;
		//division
		case 15:
			operation[i]='/';
			break;
	}

	//if a number button was pressed
	if(temp < 100) {
		num[i] *= 10;
		num[i] += temp;

		//convert int to string and then to char array
		std::string trashStr= num_to_string(num[i]);
		string_to_arr(trashStr);

		//print s[2][8] to screen
		bt->send_set_lcd(s);
	}

	//if an operation button was pressed that isn't equals or escape
	else if(operation[i] != ' ' && operation[i] != '=' && operation[i] != 'e') {
		calc_total();
		//clear screen
		bt->send_set_lcd(s);
		i++;
	}

	else if(operation[i]=='=') {
		calc_total();

		//convert int to string and then to char array
		std::string trashStr= num_to_string(num[i]);
		string_to_arr(trashStr);

		//print s[2][8] to screen
		bt->send_set_lcd(s);
		i++;
	}

	else if(operation[i]==' ') {
		//double check if this works or if the end function needs to be called explicitly
		end=1;
	}

}

//what to do when you need to update
void calculator::update()
{

	//declares a variable of type button_event
	union blue_trellis::button_event event;
	char header = bt->poll_header();
	
	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event=bt->get_button_event();
		handle_button_event(event);	
	}

	//event.button_num is how you access the button that was just pressed
	//event.is_rising, button is being released when .is_rising==1
	//read input
	//tell LCD next char
	//do whatever operation
}

//keep this returning false, better to just unplug and then replug to power than to code
bool calculator::is_done()
{
	if(end==1) {
		return true;
	}

	return false;
}

//destroy, get rid, kill scene
calculator::~calculator()
{
	//delete event;
	//Whatever gets created for this scene needs to be deleted here
}
