#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "SceneElements.h"

class DrawableObject {
public:
	virtual void Draw() = 0;
	virtual void Update() {}
	virtual void SetGui() {}
	static SceneElements* scene;
};

#endif 