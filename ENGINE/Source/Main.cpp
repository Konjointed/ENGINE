#include <iostream>

#include "Application.h"

//Entry Point
int main(int argc, char* argv[]) {
	Application application;

	if (!application.Init())
		return -1;

	return application.Run();
}