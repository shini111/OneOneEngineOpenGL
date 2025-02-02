#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <vector>
#include <algorithm>
#include <random>

// Function to generate a random float between min and max
float getRandomFloat(float min, float max);

// Function to generate a random int between min and max
int getRandomInt(int min, int max);

// Template function to remove null pointers from a vector
template <typename T>
void removeNullPointers(std::vector<T>& vec) {
	vec.erase(std::remove_if(vec.begin(), vec.end(), [](T ptr) { return ptr == nullptr; }), vec.end());
}

// Function to get the global rotation
float* GetGlobalRotation();

#endif // TOOLBOX_H