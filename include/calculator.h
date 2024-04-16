
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "scene.h"
#include "blue_trellis.h"

class calculator : public scene {
public:
	calculator(blue_trellis *bt);
	virtual void update();
	virtual bool is_done();
	virtual ~calculator();
private:
	blue_trellis *bt;
};

#endif /* CALCULATOR_H */
