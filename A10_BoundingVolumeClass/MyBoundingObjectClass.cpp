#include "MyBoundingObjectClass.h"



MyBoundingObjectClass::MyBoundingObjectClass(std::vector<vector3> vertexList)
{
	m_bColliding = false;
	m_fRadius = 0.0f;
	m_v3CenterGlobal = vector3(0.0f);

	if (vertexList.size() < 1)
		return;

	vector3 v3Min = vertexList[0];
	vector3 v3Max = vertexList[0];

	//finding 2 points, the min corner and max corner
	//front lower left & top further right
	for (int i = 1; i < vertexList.size(); i++)
	{
		if (v3Min.x > vertexList[i].x)
		{
			v3Min.x = vertexList[i].x;
		}
		else if (v3Max.x < vertexList[i].x)
		{
			v3Max.x = vertexList[i].x;
		}

		if (v3Min.y > vertexList[i].y)
		{
			v3Min.y = vertexList[i].y;
		}
		else if (v3Max.y < vertexList[i].y)
		{
			v3Max.y = vertexList[i].y;
		}

		if (v3Min.z > vertexList[i].z)
		{
			v3Min.z = vertexList[i].z;
		}
		else if (v3Max.z < vertexList[i].z)
		{
			v3Max.z = vertexList[i].z;
		}
	}

	//find the center point of obj
	m_v3CenterLocal = m_v3CenterGlobal = (v3Max + v3Min) / 2.0f;

	//give it a radius based on its max point distance
	m_fRadius = glm::distance(m_v3CenterGlobal, v3Max);

	//create instance of mesh
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}

void MyBoundingObjectClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));

}


void MyBoundingObjectClass::RenderSphere()
{
}

void MyBoundingObjectClass::RenderBox(vector3 pos)
{
}

bool MyBoundingObjectClass::IsColliding(MyBoundingObjectClass * a_other)
{
	//check for sphere collision first
	if (CheckSphereCollision(a_other)) {
		//then check for box collision
		if (CheckBoxCollision(a_other)) {
			return true;
		}
	}

	//else return false for collision
	return false;
}

bool MyBoundingObjectClass::CheckBoxCollision(MyBoundingObjectClass* a_other)
{
	if (this->m_v3MaxG.x < a_other->m_v3MinG.x)
		return false;
	if (this->m_v3MinG.x > a_other->m_v3MaxG.x)
		return false;

	if (this->m_v3MaxG.y < a_other->m_v3MinG.y)
		return false;
	if (this->m_v3MinG.y > a_other->m_v3MaxG.y)
		return false;

	if (this->m_v3MaxG.z < a_other->m_v3MinG.z)
		return false;
	if (this->m_v3MinG.z > a_other->m_v3MaxG.z)
		return false;

	//else return true
	return true;
}

bool MyBoundingObjectClass::CheckSphereCollision(MyBoundingObjectClass* a_other)
{
	float fDistance = glm::distance(this->m_v3CenterGlobal, a_other->m_v3CenterGlobal);
	float fRadiiSum = this->m_fRadius + a_other->m_fRadius;
	//return t/f
	return fDistance < fRadiiSum;

	//else return true
	return true;
}

//properties
void MyBoundingObjectClass::SetVisibility(bool value){ m_bvisible = value;}
void MyBoundingObjectClass::SetColliding(bool input) { m_bColliding = input; }
void MyBoundingObjectClass::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }
void MyBoundingObjectClass::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }
void MyBoundingObjectClass::SetRadius(float input) { m_fRadius = input; }
bool MyBoundingObjectClass::GetColliding(void) { return m_bColliding; }
vector3 MyBoundingObjectClass::GetCenterLocal(void) { return m_v3CenterLocal; }
vector3 MyBoundingObjectClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
float MyBoundingObjectClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBoundingObjectClass::GetModelMatrix(void) { return m_m4ToWorld; }

MyBoundingObjectClass::~MyBoundingObjectClass()
{
}
