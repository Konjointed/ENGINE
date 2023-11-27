#ifndef PROPERTIES_H
#define PROPERTIES_H

class Properties {
public:
	Properties();
	~Properties();

	void Draw();
private:
	char searchQuery[128] = "";
};

#endif 