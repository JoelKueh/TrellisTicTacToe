
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
	int player;
	int winner;
	int board[16];

	void check_winner(int player_num);
	void handle_button_event(union blue_trellis::button_event press);
	bool is_full();
};

#endif /* TICTACTOE_H */
