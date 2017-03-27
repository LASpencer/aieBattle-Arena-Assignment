#include "Application2D.h"

// TODO commenting
// TODO rule of 5 on all classes

int main() {
	
	auto app = new Application2D();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}