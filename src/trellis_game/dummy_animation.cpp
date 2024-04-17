
#include "dummy_animation.h"
#include <chrono>

// debug
#include <iostream>

dummy_animation::dummy_animation(blue_trellis *bt)
{
	this->bt = bt;

	start_time = new high_resolution_clock::time_point(
			high_resolution_clock::now()
	);
	last_time = new high_resolution_clock::time_point(
			high_resolution_clock::now()
	);
	last_frame = 0;

	bt->send_set_display(frames[0]);
	bt->send_show();
}



void dummy_animation::update()
{
	// Get the time between this frame and the last
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - *last_time);

	// If 500 milliseconds have passed, update the time.
	if (duration.count() > 500) {
		last_frame = !last_frame;
		bt->send_set_display(frames[last_frame]);
		bt->send_show();

		// Update our last frame time
		delete last_time;
		last_time = new high_resolution_clock::time_point(now);
	}

	// Delete button presses in the buffer so we don't have a bunch
	// queued when this animation ends.
	consume_button_presses();
}

bool dummy_animation::is_done()
{
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(now - *start_time);

	return duration.count() > 5;
}

dummy_animation::~dummy_animation()
{
	delete start_time;
	delete last_time;
}

void dummy_animation::consume_button_presses()
{
	char header = bt->poll_header();
	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		bt->get_button_event();
	}
}
