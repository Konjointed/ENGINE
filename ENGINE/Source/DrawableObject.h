#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "Scene.h"

class DrawableObject {
public:
	virtual void Draw() = 0;
	virtual void Update() {}
	virtual void SetGui() {}
	static Scene* scene;
};

#endif 