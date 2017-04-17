#pragma once

#include "MyBoundingObjectClass.h"

class BoundingBoxManagerSingleton
{
	// The single instance of the BoundingBoxManagerSingleton
	static BoundingBoxManagerSingleton* instance;

	// Basic constructor
	BoundingBoxManagerSingleton();

	// Copy constructor
	BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other) {
		instance = other.GetInstance();
	};

	// Copy assignment
	BoundingBoxManagerSingleton& operator=(BoundingBoxManagerSingleton const& other) {
		instance = other.GetInstance();
	}

	// Basic destructor
	~BoundingBoxManagerSingleton();

public:
	// Returns the single instance of BoundingBoxManagerSingleton
		// If one doesn't exist, create an instance
	static BoundingBoxManagerSingleton* GetInstance() {
		if (instance == nullptr) {
			instance = new BoundingBoxManagerSingleton();
		}
		return instance;
	};

	// Deletes the instance of BoundingBoxManagerSingleton if not already destroyed
	static void ReleaseInstance(void) {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	};

	
};
// Necessary for setting the instance as a nullptr to begin with
BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::instance = nullptr;