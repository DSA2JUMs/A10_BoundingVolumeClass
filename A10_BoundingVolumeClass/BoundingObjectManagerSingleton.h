#pragma once

#include "MyBoundingObjectClass.h"

class BoundingObjectManagerSingleton
{
	// The single instance of the BoundingObjectManagerSingleton
	static BoundingObjectManagerSingleton* instance;

	// Basic constructor
	BoundingObjectManagerSingleton();

	// Copy constructor
	BoundingObjectManagerSingleton(BoundingObjectManagerSingleton const& other) {
		instance = other.GetInstance();
	};

	// Copy assignment
	BoundingObjectManagerSingleton& operator=(BoundingObjectManagerSingleton const& other) {
		instance = other.GetInstance();
	}

	// Basic destructor
	~BoundingObjectManagerSingleton();

public:
	// Returns the single instance of BoundingObjectManagerSingleton
	// If one doesn't exist, create an instance
	static BoundingObjectManagerSingleton* GetInstance() {
		if (instance == nullptr) {
			instance = new BoundingObjectManagerSingleton();
		}
		return instance;
	};

	// Deletes the instance of BoundingObjectManagerSingleton if not already destroyed
	static void ReleaseInstance(void) {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	};


};
// Necessary for setting the instance as a nullptr to begin with
BoundingObjectManagerSingleton* BoundingObjectManagerSingleton::instance = nullptr;