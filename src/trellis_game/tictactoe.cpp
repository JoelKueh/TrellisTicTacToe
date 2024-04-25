
#include "tictactoe.h"

#define PONE 0
#define PTWO 1

#define FULLONE 0
#define FULLTWO 1
#define EMPTY 2
#define TIE 3

tictactoe::tictactoe(blue_trellis *bt) {
	this->bt = bt;

	end=false; 
	player=PONE;
	winner=EMPTY;

	//clear lcd display
	uint8_t lcd_empty[2][8] = {
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};
	bt->send_set_lcd(lcd_empty);

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

bool tictactoe::is_full() {
	for(int i=0; i<16; i++) {
		if(board[i]==EMPTY) {
			return false;
		}
	}

	return true;
}

void tictactoe::check_winner(int player_num) {
	//check rows for a win
	for(int i=0; i<=12; i=i+4) {
		if(player_num ==  board[i] && board[i] == board[i+1] && board[i+1] == board[i+2] && board[i+2] == board[i+3]) {
			winner=player_num;
		}
	}
	//check columns for a win
	for(int i=0; i<=3; i++) {
		if(player_num ==  board[i] && board[i] == board[i+4] && board[i+4] == board[i+8] && board[i+8] == board[i+12]) {
			winner=player_num;
		}
	}
	//check right diagonal for win
	if(player_num ==  board[0] && board[0] == board[5] && board[5] == board[10] && board[10] == board[15]) {
		winner=player_num;
	}
	//check left diagonal for win
	else if(player_num ==  board[3] && board[3] == board[6] && board[6] == board[9] && board[9] == board[12]) {
		winner=player_num;
	}
	else if(is_full()) {
		winner=TIE;
	}

	uint8_t one_wins[2][8] = {
		{ 'P', 'l', 'a', 'y', 'e', 'r', ' ', ' ' },
		{ 'O', 'n', 'e', ' ', 'W', 'i', 'n', 's' }
	};

	uint8_t two_wins[2][8] = {
		{ 'P', 'l', 'a', 'y', 'e', 'r', ' ', ' ' },
		{ 'T', 'w', 'o', ' ', 'W', 'i', 'n', 's' }
	};

	uint8_t no_one_wins[2][8] = {
		{ 'n', 'o', ' ', 'o', 'n', 'e', ' ', ' ' },
		{ 'w', 'i', 'n', 's', ' ', ':', '(', ' ' }
	};

	if(winner == PONE) {
		bt->send_set_lcd(one_wins);
		player=2;
	}
	else if(winner == PTWO) {
		bt->send_set_lcd(two_wins);
		player=2;
	}
	else if(winner == TIE) {
		bt->send_set_lcd(no_one_wins);
		player=2;
	}

	return;
}

void tictactoe::handle_button_event(union blue_trellis::button_event press) {
	//only count button presses, not releases
	if (press.is_rising) {
		return;
	}

	if(winner != EMPTY) {
		end=true;
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

		//switch players
		player=!player;
		//player=(player+1)%2;

		//check to see if the player who just played won
		check_winner(!player);
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
