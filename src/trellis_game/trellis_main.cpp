
#include <string>

#include "blue_trellis.h"
#include "scene.h"
#include "dummy_animation.h"
#include "tictactoe.h"
#include "mole_whacker.h"
#include "calculator.h"

const std::string bluetooth_addr = "00:14:03:05:5E:92";
//const std::string bluetooth_addr = "98:D3:31:FD:73:7F";

scene *get_scene(blue_trellis *bt, union blue_trellis::button_event event)
{
	if (event.is_rising)
		return nullptr;

	switch (event.button_num) {
	case 0:
		return new tictactoe(bt);
	case 1:
		return new mole_whacker(bt);
	case 2:
		return new calculator(bt);
	case 3:
		return new dummy_animation(bt);
	default:
		return nullptr;
	}
}

void reset_scene(blue_trellis &bt)
{
	const uint8_t select_frame[16][3] = {
		{ 0x80, 0x00, 0x00 },
		{ 0x00, 0x80, 0x00 },
		{ 0x00, 0x00, 0x80 },
		{ 0x80, 0x80, 0x80 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00 }
	};

	const uint8_t blank_lcd[2][8] = {
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
		{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};

	bt.send_set_display(select_frame);
	bt.send_set_lcd(blank_lcd);
}

scene *select_scene(blue_trellis &bt)
{
	char header = bt.poll_header();
	union blue_trellis::button_event event;

	if (header == blue_trellis::BUTTON_EVENT_HEADER) {
		event = bt.get_button_event();
		return get_scene(&bt, event);
	}

	return nullptr;
}

int main()
{
	blue_trellis bt = blue_trellis(bluetooth_addr);
	scene *active_scene = nullptr;
	reset_scene(bt);

	while (true) {
		// If we don't have a scene, try to select one.
		if (!active_scene) {
			active_scene = select_scene(bt);
			continue;
		}

		// If we have a scene pass controll to that scene.
		active_scene->update();
		if (active_scene->is_done()) {
			reset_scene(bt);
			delete active_scene;
			active_scene = nullptr;
		}
	}
}
