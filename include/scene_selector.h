
#include "scene.h"

class scene_selector : private scene {
	scene_selector();
	virtual void update();
	virtual bool is_done();
	virtual ~scene_selector();
};


