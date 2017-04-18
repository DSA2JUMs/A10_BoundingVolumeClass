#pragma once

#include "MyBoundingObjectClass.h"

class BoundingObjectManagerSingleton
{
	// The single instance of the BoundingObjectManagerSingleton
	static BoundingObjectManagerSingleton* instance;

	// List of objects the singleton creates and has control over
	std::vector<MyBoundingObjectClass> objectList;

	// Basic constructor
	BoundingObjectManagerSingleton(void) {
		objectList = std::vector<MyBoundingObjectClass>();
	};

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

	// Used to create new Bounding Objects from a list of vertices
	void CreateBoundingObject(std::vector<vector3> vertexList) {
		objectList.push_back(MyBoundingObjectClass(vertexList));
	};

	// Loops through the objectList and checks for collisions
	void CheckCollisions() {
		for (int i = 0; i < objectList.size(); i++) {
			objectList[i].SetColliding(false);
		}
		for (int i = 0; i < objectList.size(); i++) {
			for (int j = i; j < objectList.size(); j++) {
				if (objectList[i].IsColliding(&objectList[j])) {
					objectList[i].SetColliding(true);
					objectList[j].SetColliding(true);
				}
			}
		}
	};

	void UpdatePositions(matrix4 mat, int i) {
		objectList[i].SetModelMatrix(mat);
	};

};
