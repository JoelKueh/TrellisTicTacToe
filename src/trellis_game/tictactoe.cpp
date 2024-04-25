
#include "tictactoe.h"

#define PONE 0
#define PTWO 1

#define FULLONE 0
#define FULLTWO 1
#define EMPTY 2

tictactoe::tictactoe(blue_trellis *bt) {
	this->bt = bt;

	end=false; 
	player=PONE;
	winner=EMPTY;

	//clear board
	for(int i=0; i<16; i++) {
		board[i]=EMPTY;
	}

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

void tictactoe::check_winner(bool player_num) {
	//check rows for a win
	for(int i=0; i<=12; i=i+4) {
		if(board[i]==board[i+1] && board[i+1]==board[i+2] && board[i+2]==board[i+3]) {
			winner=player_num;
		}
	}
	//check columns for a win
	for(int i=0; i<=3; i++) {
		if(board[i] == board[i+4] && board[i+4] == board[i+8] && board[i+8] == board[i+12]) {
			winner=player_num;
		}
	}
	//check right diagonal for win
	if(board[0] == board[5] && board[5] == board[10] && board[10] == board[15]) {
		winner=player_num;
	}
	//check left diagonal for win
	else if(board[3]==board[6] && board[6]==board[9] && board[9]==board[12]) {
		winner=player;
	}

	uint8_t one_wins[2][8] = {
		{ 'P', 'l', 'a', 'y', 'e', 'r', ' ', ' ' },
		{ 'O', 'n', 'e', ' ', 'W', 'i', 'n', 's' }
	};

	uint8_t two_wins[2][8] = {
		{ 'P', 'l', 'a', 'y', 'e', 'r', ' ', ' ' },
		{ 'T', 'w', 'o', ' ', 'W', 'i', 'n', 's' }
	};

	if(winner == PONE) {
		bt->send_set_lcd(one_wins);
	}
	else if(winner == PTWO) {
		bt->send_set_lcd(two_wins);
	}

	return;
}

void tictactoe::handle_button_event(union blue_trellis::button_event press) {
	//only count button presses, not releases
	if (press.is_rising) {
		return;
	}

	if(board[press.button_num] == EMPTY) {
		if(player==PONE) {
			bt->send_set_led(press.button_num, 0x30, 0xD5, 0xC8); //pinkish-purple
			board[press.button_num]=FULLONE;
		}
		else {
			bt->send_set_led(press.button_num, 0xD5, 0x30, 0xC8); //turquoise
			board[press.button_num]=FULLTWO;
		}

		//check to see if the current player won
		check_winner(player);

		//since no one won, switch players
		player=!player;
	}
	
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
