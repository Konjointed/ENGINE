#ifndef DRAWABLE_OBJECT_H
#define DRAWABLE_OBJECT_H

#include "GameObject.h"
#include "Scene.h"

class DrawableObject : public GameObject {
public:
	virtual void Draw() {}
	virtual void SetGui() {}
	static Scene* scene;
};

#endif 