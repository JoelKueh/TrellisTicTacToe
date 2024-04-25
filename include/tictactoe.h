
#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "scene.h"
#include "blue_trellis.h"

class tictactoe : public scene {
public:
	tictactoe(blue_trellis *bt);
	virtual void update();
	virtual bool is_done();
	virtual ~tictactoe();
private:
	blue_trellis *bt;
	bool end;
	bool player;
	bool winner;
	int board[16];

	void check_winner(bool player_num);
	void handle_button_event(union blue_trellis::button_event press);
};

#endif /* TICTACTOE_H */
