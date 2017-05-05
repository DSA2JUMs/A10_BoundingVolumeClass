#pragma once
#include "RE\ReEng.h"
#include "MyBoundingObjectClass.h"
class MyOctree
{
public:
	
	std::vector<MyOctree> children;
	std::vector<MyBoundingObjectClass*> object;


	MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth);
	void CheckColisions();
	void AddObject(MyBoundingObjectClass *iobject);

	void SetSOVisibilty(bool value);
	bool GetSOVisibilty();
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
	int depth;
};

