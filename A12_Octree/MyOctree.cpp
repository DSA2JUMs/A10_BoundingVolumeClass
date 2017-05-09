#include "MyOctree.h"


//but I do not see any logic on why keep subdividing the active octant in the first place and so -alberto

MyOctree::MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth)
{
	m_pMeshMngr = MeshManagerSingleton::GetInstance();

	minX = iminx;
	maxX = imaxx;
	cx = (minX + maxX) / 2;
	minY = iminy;
	maxY = imaxy;
	cy = (minY + maxY) / 2;
	minZ = iminz;
	maxZ = imaxz;
	cz = (minZ + maxZ) / 2;

	hasChildren = false;

	depth = idepth;

	object = {};

	std::vector<vector3> vecList;

	vecList.push_back(vector3(minX, minY, minZ));
	vecList.push_back(vector3(maxX, maxY, maxZ));

	boundingBox = MyBoundingObjectClass(vecList);
}

void MyOctree::Divide()
{
	// Make new 8 children
	children.push_back(MyOctree(minX, cx, minY, cy, minZ, cz, depth - 1));
	children.push_back(MyOctree(cx, maxX, minY, cy, minZ, cz, depth - 1));
	children.push_back(MyOctree(minX, cx, cy, maxY, minZ, cz, depth - 1));
	children.push_back(MyOctree(minX, cx, minY, cy, cz, maxZ, depth - 1));
	children.push_back(MyOctree(cx, maxX, cy, maxY, minZ, cz, depth - 1));
	children.push_back(MyOctree(minX, cx, cy, maxY, cz, maxZ, depth - 1));
	children.push_back(MyOctree(cx, maxX, minY, cy, cz, maxZ, depth - 1));
	children.push_back(MyOctree(cx, maxX, cy, maxY, cz, maxZ, depth - 1));

	// It now has children, congratulations you're a father
	hasChildren = true;

	// Adds the objects to the children - since it now has children it will find the appropriate child and add the object to that child
	for (int i = 0; i < object.size(); i++) {
		AddObject(object[i]);
	}
	// Removes all objects from the parent - this one!
	object.clear();
}

void MyOctree::CheckCollisions()
{
	if (hasChildren)
	{
		//checking for lowest level of tree
		for (int i = 0; i < 8; i++)
		{
			children[i].CheckCollisions(); //recursive 
		}
	}
	else
	{
		for (int i = 0; i < object.size(); i++) {
			object[i].SetColliding(false);
		}

		//once you get to the lowest level, check for collision
		for (int i = 0; i < object.size(); i++)
		{
			for (int j = i; j < object.size(); j++)
			{
				if (i != j)
				{
					if (object[i].IsColliding(&object[j])) {

						object[i].SetColliding(true);
						object[j].SetColliding(true);
						//checking collisions
					}
				}

			}
		}
	}
}

void MyOctree::AddObject(MyBoundingObjectClass iobject)
{
	// If this has no children...
	if (!hasChildren) {
		// If it exceed the number of objects it can hold, make some chilluns to hold the objects for it
		if (object.size() > m_iMAX_OBJECTS && depth > 0)
		{
			//std::cout << "OBJECT SIZE: (ADDOBJECT) " << object.size() << " MAX OBJECTS: " << m_iMAX_OBJECTS << " DEPTH: " << depth << std::endl;
			Divide();
		}
		else {
			// Add the object to this octree
			object.push_back(iobject);

		}
	}
	// ...else if children exist
	else
	{
		for (int i = 0; i < children.size(); i++) {
			if (iobject.CheckBoxCollision(&children[i].boundingBox)) {
				children[i].AddObject(iobject);
			}
		}
	}
}

void MyOctree::SetOctreeVis(bool value) { 
	octreeVis = value;
	if (hasChildren)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i].SetOctreeVis(value);
		}
	}
}

bool MyOctree::GetOctreeVis() { return octreeVis; }

void MyOctree::SetSOCheck(bool value) {
	SOCheck = value;
	if (hasChildren) {
		for (int i = 0; i < 8; i++) {
			children[i].SetSOCheck(value);
		}
	}
}

bool MyOctree::GetSOCheck() { return SOCheck; }

void MyOctree::Render()
{
	//add box to render list
	m_pMeshMngr->AddCubeToRenderList(
		glm::translate(vector3(cx, cy, cz)) *
		glm::scale(vector3(maxX - minX, maxY - minY, maxZ - minZ)),
		REPURPLE, WIRE);

	if (hasChildren)
	{
		for each (MyOctree child in children)
		{
			child.Render();
		}
	}
}

MyOctree::~MyOctree()
{
}
