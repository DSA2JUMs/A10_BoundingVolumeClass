#include "MyOctree.h"

MyOctree::MyOctree(float iminx, float imaxx, float iminy, float imaxy, float iminz, float imaxz, int idepth)
{
	minX = iminx;
	maxX = imaxx;
	cx = (minX + maxX) / 2;
	minY = iminy;
	maxY = imaxy;
	cy = (minY + maxY) / 2;
	minZ = iminz;
	maxZ = imaxz;
	cz = (minZ + maxZ) / 2;
	depth = idepth;

	if (depth > 0)
	{
		children.push_back(MyOctree(minX, cx, minY, cy, minZ, cz, depth - 1));
		children.push_back(MyOctree(cx, maxX, minY, cy, minZ, cz, depth - 1));
		children.push_back(MyOctree(minX, cx, cy, maxY, minZ, cz, depth - 1));
		children.push_back(MyOctree(minX, cx, minY, cy, cz, maxZ, depth - 1));
		children.push_back(MyOctree(cx, maxX, cy, maxY, minZ, cz, depth - 1));
		children.push_back(MyOctree(minX, cx, cy, maxY, cz, maxZ, depth - 1));
		children.push_back(MyOctree(cx, maxX, minY, cy, cz, maxZ, depth - 1));
		children.push_back(MyOctree(cx, maxX, cy, maxY, cz, maxZ, depth - 1));
		hasChildren = true;
	}
	else
	{
		hasChildren = false;
	}
}

void MyOctree::CheckColisions()
{
	if (hasChildren)
	{
		//checking for lowest level of tree
		for (int i = 0; i < 8; i++)
		{
			children[i].CheckColisions(); //recursive 
		}
	}
	else
	{
		//once you get to the lowest level, check for collision
		for (int i = 0; i < object.size(); i++)
		{
			for (int j = 0; j < object.size(); j++)
			{
				if (i != j)
				{
					if (object[i]->IsColliding(object[j])) {

						object[i]->SetColliding(true);
						object[j]->SetColliding(true);
						//checking collisions
					}
				}

			}
		}
	}
}

void MyOctree::AddObject(MyBoundingObjectClass * iobject)
{
	object.push_back(iobject);

	if (hasChildren)
	{
		for (int i = 0; i < 8; i++)
		{
			if (children[i].minX <= iobject->GetMin().x &&  iobject->GetMin().x <= children[i].maxX)
			{
				if (children[i].minY <= iobject->GetMin().y &&  iobject->GetMin().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject->GetMin().z &&  iobject->GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
					else if (children[i].minZ <= iobject->GetMax().z &&  iobject->GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
				}
				else if (children[i].minY <= iobject->GetMax().y &&  iobject->GetMax().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject->GetMin().z &&  iobject->GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
					else if (children[i].minZ <= iobject->GetMax().z &&  iobject->GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
				}
			}
			else if (children[i].minX <= iobject->GetMax().x &&  iobject->GetMax().x <= children[i].maxX)
			{
				if (children[i].minY <= iobject->GetMin().y &&  iobject->GetMin().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject->GetMin().z &&  iobject->GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
					else if (children[i].minZ <= iobject->GetMax().z &&  iobject->GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
				}
				else if (children[i].minY <= iobject->GetMax().y &&  iobject->GetMax().y <= children[i].maxY)
				{
					if (children[i].minZ <= iobject->GetMin().z &&  iobject->GetMin().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
					}
					else if (children[i].minZ <= iobject->GetMax().z &&  iobject->GetMax().z <= children[i].maxZ)
					{
						children[i].AddObject(iobject);
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

MyOctree::~MyOctree()
{
}
