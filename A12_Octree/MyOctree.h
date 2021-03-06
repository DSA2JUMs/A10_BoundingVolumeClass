#pragma once
#include "RE\ReEng.h"
#include "MyBoundingObjectClass.h"
class MyOctree
{
public:
	
	std::vector<MyOctree> children;
	std::vector<MyBoundingObjectClass> object;


	MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth);

	// Divides this cube into 8 smaller cubes.  Moves any game objects that are completely contained within the new smaller cubes into those cubes and removes them from this one.
	void Divide();

	void CheckCollisions();

	// Adds a game object to the cube.  If the cube has too many objects in it, and hasn't been divided already, it should be divided.
	void AddObject(MyBoundingObjectClass& iobject);

	void SetOctreeVis(bool value);
	bool GetOctreeVis();

	void SetSOCheck(bool value);
	bool GetSOCheck();

	void Render();

	~MyOctree();

private:
	float minX;
	float maxX;
	float cx;
	float minY;
	float maxY;
	float cy;
	float minZ;
	float maxZ;
	float cz;
	bool hasChildren;
	bool octreeVis = false;
	bool SOCheck = true;
	int depth;
	MeshManagerSingleton* m_pMeshMngr = nullptr;

	MyBoundingObjectClass boundingBox;

	const static int m_iMAX_OBJECTS = 5;

};