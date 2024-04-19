
#ifndef DUMMY_ANIMATION_H
#define DUMMY_ANIMATION_H

#include "scene.h"
#include "blue_trellis.h"

#include <chrono>
using namespace std::chrono;

class dummy_animation : public scene {
public:
	dummy_animation(blue_trellis *bt);
	virtual void update();
	virtual bool is_done();
	virtual ~dummy_animation();
private:
	blue_trellis *bt;
	high_resolution_clock::time_point *start_time;
	high_resolution_clock::time_point *last_time;
	int frame_count;

	void draw_frame(int number);

	void consume_button_presses();
};

#endif /* DUMMY_ANIMATION_H */
