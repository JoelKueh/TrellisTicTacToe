
#ifndef MOLE_WHACKER_H
#define MOLE_WHACKER_H

#include "scene.h"
#include "blue_trellis.h"

class mole_whacker : public scene {
public:
	mole_whacker(blue_trellis *bt);
	virtual void update();
	virtual bool is_done();
	virtual ~mole_whacker();
private:
	blue_trellis *bt;
	int selected_mole;
	int game_score;
	high_resolution_clock::time_point *start_time;
	high_resolution_clock::time_point *last_time;

	void set_rand_mole();
	void handle_button_press(union blue_trellis::button_event press);
	std::string num_to_string(int num);
	void update_lcd(std::string num);

};

#endif /* MOLE_WHACER_H */
