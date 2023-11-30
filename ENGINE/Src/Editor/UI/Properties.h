#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "../../GameObject.h"
#include "../../Camera.h"

class Properties {
public:
	Properties();
	~Properties();

	void CameraProperties(Camera* camera);
	void ObjectProperties(GameObject* object);
	void Draw(GameObject* selectedObject);
private:
	char searchQuery[128] = "";
};

#endif 