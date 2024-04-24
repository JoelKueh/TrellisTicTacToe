
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
	int num_buff[2];
	bool end;
	bool is_second_num;
	char last_op;

	void handle_button_event(union blue_trellis::button_event press);
	float calc_total(float num1, float num2, char op);
	void update_lcd(std::string num);
	std::string num_to_string(float num);
	std::string num_to_string(int num);
};

#endif /* CALCULATOR_H */
