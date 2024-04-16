
#ifndef SCENE_H
#define SCENE_H

/**
 * Template class for other trellis games
 */
class scene {
public:
	scene() = default;
	virtual void update() = 0;
	virtual bool is_done() = 0;
	virtual ~scene() = default;
private:
};

#endif /* SCENE_H */
