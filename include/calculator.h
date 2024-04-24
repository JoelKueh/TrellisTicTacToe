
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
	void handle_button_event(union blue_trellis::button_event press);
	void calc_total();
	void string_to_arr(std::string input);
	std::string num_to_string(float num);
	std::string num_to_string(int num);


};

#endif /* CALCULATOR_H */
