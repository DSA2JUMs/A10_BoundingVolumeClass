#pragma once
#include "RE\ReEng.h"
#include "MyBoundingObjectClass.h"
class MyOctree
{
public:
	
	std::vector<MyOctree> children;
	std::vector<MyBoundingObjectClass*> object;


	MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth);
	void CheckCollisions();
	void AddObject(MyBoundingObjectClass *iobject);

	void SetSOVisibility(bool value);
	bool GetSOVisibility();

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
	bool SOVisibility = false;
	bool SOCheck = true;
	int depth;
	MeshManagerSingleton* m_pMeshMngr = nullptr;
};

