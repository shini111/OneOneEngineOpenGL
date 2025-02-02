#include "ToolBox.h"


extern float globalRotation; // Declare the external global rotation variable


// Function to generate a random float between min and max
float getRandomFloat(float min, float max) {
	static std::default_random_engine engine{ std::random_device{}() };
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(engine);
}

// Function to generate a random int between min and max
int getRandomInt(int min, int max) {
	static std::default_random_engine engine{ std::random_device{}() };
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(engine);
}

// Function to get the global rotation
float* GetGlobalRotation() {
	return &globalRotation;
}