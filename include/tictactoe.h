
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
};

#endif /* TICTACTOE_H */
