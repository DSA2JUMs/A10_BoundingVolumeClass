#pragma once
#include "RE\ReEng.h"
#include "MyBoundingObjectClass.h"
class MyOctree
{
public:
	int depth;
	std::vector<MyOctree> children;
	std::vector<MyBoundingObjectClass*> object;
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

	MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth);
	void CheckColisions();
	void AddObject(MyBoundingObjectClass *iobject);
	~MyOctree();
};

