#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <vector>

class Console {
public:
	Console();
	~Console();

	void Draw();
	void AddLog(const std::string& log);
private:
	char inputBuf[256] = "";
	char filterBuf[256] = "";
	std::vector<std::string> output;

	void ExecuteCommand(const std::string& command);
	bool IsLineVisible(const std::string& line);
};

#endif 