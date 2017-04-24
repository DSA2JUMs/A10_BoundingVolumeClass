#include "MyBoundingObjectClass.h"

MyBoundingObjectClass::MyBoundingObjectClass(std::vector<vector3> vertexList)
{
	m_bColliding = false;
	m_fRadius = 0.0f;
	m_v3CenterGlobal = vector3(0.0f);

	if (vertexList.size() < 1)
		return;

	//finding the min corner and max corner
	m_v3Min = vertexList[0];
	m_v3Max = vertexList[0];

	for (int i = 1; i < vertexList.size(); i++)
	{
		if (m_v3Min.x > vertexList[i].x)
		{
			m_v3Min.x = vertexList[i].x;
		}
		else if (m_v3Max.x < vertexList[i].x)
		{
			m_v3Max.x = vertexList[i].x;
		}

		if (m_v3Min.y > vertexList[i].y)
		{
			m_v3Min.y = vertexList[i].y;
		}
		else if (m_v3Max.y < vertexList[i].y)
		{
			m_v3Max.y = vertexList[i].y;
		}

		if (m_v3Min.z > vertexList[i].z)
		{
			m_v3Min.z = vertexList[i].z;
		}
		else if (m_v3Max.z < vertexList[i].z)
		{
			m_v3Max.z = vertexList[i].z;
		}
	}

	m_v3CenterLocal = m_v3CenterGlobal = (m_v3Max + m_v3Min) / 2.0f;
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_v3Size = m_v3Max - m_v3Min;

	m_v3MaxG = m_v3Max;
	m_v3MinG = m_v3Min;

	m_v3MaxSurrounding = m_v3Max;
	m_v3MinSurrounding = m_v3Min;

	m_v3SurroundingSize = m_v3MaxSurrounding - m_v3MinSurrounding;
}

void MyBoundingObjectClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;


	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));

	m_v3MinG = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	m_v3MaxG = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));

	std::vector<vector3> vectorList;

	//8 local points
	vectorList.push_back(vector3(m_v3Min.x, m_v3Min.y, m_v3Min.z)); //min
	vectorList.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z));
	vectorList.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z));
	vectorList.push_back(vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z));

	vectorList.push_back(vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z));
	vectorList.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z));
	vectorList.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z));
	vectorList.push_back(vector3(m_v3Max.x, m_v3Max.y, m_v3Max.z)); //max

																	//turn all 8 points to global
	for (int i = 0; i < 8; i++)
	{
		vectorList[i] = vector3(m_m4ToWorld * vector4(vectorList[i], 1.0f));
	}

	//set minLargest and maxLargest to the first point
	m_v3MaxSurrounding = vectorList[0];
	m_v3MinSurrounding = m_v3MaxSurrounding;

	for (int i = 1; i < 8; i++)
	{
		if (m_v3MinSurrounding.x > vectorList[i].x)
		{
			m_v3MinSurrounding.x = vectorList[i].x;
		}
		else if (m_v3MaxSurrounding.x < vectorList[i].x)
		{
			m_v3MaxSurrounding.x = vectorList[i].x;
		}

		if (m_v3MinSurrounding.y > vectorList[i].y)
		{
			m_v3MinSurrounding.y = vectorList[i].y;
		}
		else if (m_v3MaxSurrounding.y < vectorList[i].y)
		{
			m_v3MaxSurrounding.y = vectorList[i].y;
		}

		if (m_v3MinSurrounding.z > vectorList[i].z)
		{
			m_v3MinSurrounding.z = vectorList[i].z;
		}
		else if (m_v3MaxSurrounding.z < vectorList[i].z)
		{
			m_v3MaxSurrounding.z = vectorList[i].z;
		}
	}

	m_v3SurroundingSize = m_v3MaxSurrounding - m_v3MinSurrounding;
	if (m_fRadius < m_v3SurroundingSize.x)
	{
		m_fRadius = m_v3SurroundingSize.x;
	}
	if (m_fRadius < m_v3SurroundingSize.y)
	{
		m_fRadius = m_v3SurroundingSize.y;
	}
	if (m_fRadius < m_v3SurroundingSize.z)
	{
		m_fRadius = m_v3SurroundingSize.z;
	}
}


void MyBoundingObjectClass::RenderSphere()
{
	vector3 v3Color = GetSphereColor();
	int value = 0; //NONE

	if (GetGeneralVisibility()) {
		value = 2; //WIRE
	}

	if (true == m_bColliding)
		v3Color = RERED;

	m_pMeshMngr->AddSphereToRenderList(
		glm::translate(m_v3CenterGlobal) *
		glm::scale(vector3(m_fRadius/2) * 2.0f), v3Color, value);
}

void MyBoundingObjectClass::RenderBox()
{
	vector3 v3Color = GetColor();
	int value = 0; //NONE

	if (GetGeneralVisibility()) {
		value = 2; //WIRE
	}

	if (true == m_bColliding)
		v3Color = RERED;

	if (value == 2) {
		if (!GetAABBVisibility())
		{
			value = 0;
		}

		// Personal cube
		m_pMeshMngr->AddCubeToRenderList(
			m_m4ToWorld *
			glm::translate(m_v3CenterLocal) *
			glm::scale(m_v3Size),
			v3Color, value);

		// All-encompassing cube
		m_pMeshMngr->AddCubeToRenderList(
			glm::translate(m_v3CenterGlobal) *
			glm::scale(m_v3SurroundingSize),
			REGREEN, value);
	}
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
	float fDistance = glm::distance(m_v3CenterGlobal, a_other->m_v3CenterGlobal);
	float fRadiiSum = m_fRadius + a_other->m_fRadius;
	//return t/f
	return fDistance < fRadiiSum;

	//else return true
	return true;
}

//properties
void MyBoundingObjectClass::SetColor(vector3 color) { objColor = color; }
vector3 MyBoundingObjectClass::GetColor(void) { return objColor; }
void MyBoundingObjectClass::SetSphereColor(vector3 color) { ObjSphereColor = color; }
vector3 MyBoundingObjectClass::GetSphereColor(void) { return ObjSphereColor; }
void MyBoundingObjectClass::SetGeneralVisibility(bool value){ m_bVisible = value;}
bool MyBoundingObjectClass::GetGeneralVisibility(void) { return m_bVisible; }
void MyBoundingObjectClass::SetAABBVisibility(bool value) { m_AABBVisible = value; }
bool MyBoundingObjectClass::GetAABBVisibility(void) { return m_AABBVisible; }
void MyBoundingObjectClass::SetColliding(bool input) { m_bColliding = input; }
bool MyBoundingObjectClass::GetColliding(void) { return m_bColliding; }
void MyBoundingObjectClass::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }
void MyBoundingObjectClass::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }
vector3 MyBoundingObjectClass::GetCenterLocal(void) { return m_v3CenterLocal; }
vector3 MyBoundingObjectClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
void MyBoundingObjectClass::SetRadius(float input) { m_fRadius = input; }
float MyBoundingObjectClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBoundingObjectClass::GetModelMatrix(void) { return m_m4ToWorld; }

MyBoundingObjectClass::~MyBoundingObjectClass()
{
}
