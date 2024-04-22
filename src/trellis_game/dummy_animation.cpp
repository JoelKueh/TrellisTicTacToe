
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
	draw_frame(0);
	frame_count = 1;
}

void dummy_animation::update()
{
	// Get the time between this frame and the last
	auto now = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(now - *last_time);

	// If 50 milliseconds have passed, update the time.
	if (duration.count() > 50) {
		draw_frame(frame_count++);

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

void dummy_animation::draw_frame(int number)
{
	const uint8_t color[3] = { 0xFF, 0xFF, 0xFF };
	const uint8_t numbers[12] = { 0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4};
	uint8_t frame[16][3] = { 0 };
	int i, j, end;

	i = number % 12;
	j = 11;
	end = (number + 11) % 12;
	while (i != end) {
		frame[numbers[i]][0] = color[0] / j;
		frame[numbers[i]][1] = color[1] / j;
		frame[numbers[i]][2] = color[2] / j;
		i = i + 1 == 12 ? 0 : i + 1;
		j--;
	}
	bt->send_set_display(frame);
}
