#include "MyOctree.h"


//but I do not see any logic on why keep subdividing the active octant in the first place and so -alberto

MyOctree::MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz)
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

	object = {};
}

void MyOctree::Divide()
{
	// Make new 8 children
	children.push_back(MyOctree(minX, cx, minY, cy, minZ, cz));
	children.push_back(MyOctree(cx, maxX, minY, cy, minZ, cz));
	children.push_back(MyOctree(minX, cx, cy, maxY, minZ, cz));
	children.push_back(MyOctree(minX, cx, minY, cy, cz, maxZ));
	children.push_back(MyOctree(cx, maxX, cy, maxY, minZ, cz));
	children.push_back(MyOctree(minX, cx, cy, maxY, cz, maxZ));
	children.push_back(MyOctree(cx, maxX, minY, cy, cz, maxZ));
	children.push_back(MyOctree(cx, maxX, cy, maxY, cz, maxZ));

	// It now has children, congratulations you're a father
	hasChildren = true;

	// Adds the objects to the children - since it now has children it will find the appropriate child and add the object to that child
	for (int i = 0; i < object.size(); i++) {
		std::cout << "OBJECT SIZE: (DIVIDE) " << object.size() << std::endl;
		AddObject(object[i]);
		std::cout << "OBJECT " << i << std::endl;
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
		if (object.size() > m_iMAX_OBJECTS)
		{
			std::cout << "OBJECT SIZE: (ADDOBJECT) " << object.size() << " MAX OBJECTS: " << m_iMAX_OBJECTS << std::endl;
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
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i].minX <= iobject.GetMin().x &&  iobject.GetMin().x <= children[i].maxX)
			{
				if (children[i].minY <= iobject.GetMin().y &&  iobject.GetMin().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject.GetMin().z &&  iobject.GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
					else if (children[i].minZ <= iobject.GetMax().z &&  iobject.GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
				}
				else if (children[i].minY <= iobject.GetMax().y &&  iobject.GetMax().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject.GetMin().z &&  iobject.GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
					else if (children[i].minZ <= iobject.GetMax().z &&  iobject.GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
				}
			}
			else if (children[i].minX <= iobject.GetMax().x &&  iobject.GetMax().x <= children[i].maxX)
			{
				if (children[i].minY <= iobject.GetMin().y &&  iobject.GetMin().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject.GetMin().z &&  iobject.GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
					else if (children[i].minZ <= iobject.GetMax().z &&  iobject.GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
				}
				else if (children[i].minY <= iobject.GetMax().y &&  iobject.GetMax().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject.GetMin().z &&  iobject.GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
					else if (children[i].minZ <= iobject.GetMax().z &&  iobject.GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
						break;
					}
				}
			}
		}
	}
}

void MyOctree::SetSOVisibility(bool value) { 
	SOVisibility = value; 
	if (hasChildren)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i].SetSOVisibility(value);
		}
	}
}

bool MyOctree::GetSOVisibility() { return SOVisibility; }

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
