#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "../../GameObject.h"
#include "../../Camera.h"

struct Scene;

class Properties {
public:
	Properties();
	~Properties();

	void CameraProperties(Camera* camera);
	void ObjectProperties(GameObject* object);
	void Draw(GameObject* selectedObject, Scene& scene);
private:
	char searchQuery[128] = "";
};

#endif 