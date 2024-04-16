
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
};

#endif /* MOLE_WHACER_H */
