#pragma once
#include "Engine.h"
#include <random>

class MyGameEngine
{
private:
    // Private constructor to prevent external instantiation
    MyGameEngine() = default;
    ~MyGameEngine() = default;

    // Static pointer to hold the instance of the singleton
    static MyGameEngine* instance;

    // A member variable for your engine
    GameEngine::Engine myEngine;

public:
    // Public method to access the singleton instance
    static MyGameEngine* GetInstance()
    {
        // If the instance is null, create it
        if (instance == nullptr) {
            instance = new MyGameEngine();
        }
        return instance;
    }

    // Return the GameEngine instance
    GameEngine::Engine GetEngine()
    {
        return myEngine;
    }

    // Prevent copy construction and assignment
    MyGameEngine(const MyGameEngine&) = delete;
    MyGameEngine& operator=(const MyGameEngine&) = delete;

    // Random number functions
    static float getRandomFloat(float min, float max) {
        static std::default_random_engine engine{ std::random_device{}() };
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(engine);
    }

    static int getRandomInt(int min, int max) {
        static std::default_random_engine engine{ std::random_device{}() };
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(engine);
    }
};

// Initialize the static instance pointer to nullptr
MyGameEngine* MyGameEngine::instance = nullptr;
