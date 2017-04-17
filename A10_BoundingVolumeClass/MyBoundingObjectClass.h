#pragma once
#include "RE\ReEng.h"

class MyBoundingObjectClass
{
public:
	MyBoundingObjectClass();
	~MyBoundingObjectClass();

	MyBoundingObjectClass(std::vector<vector3> vertexList); //Constructor, needs a vertex list

	void SetCenterLocal(vector3 input); //Sets Center of the sphere in local space
	vector3 GetCenterLocal(void); //Gets center of the sphere in local space

	void SetCenterGlobal(vector3 input); //Sets Center of the sphere in global space
	vector3 GetCenterGlobal(void); //Gets center of the sphere in global space

	void SetColliding(bool input); //Sets Colliding
	bool GetColliding(void); //Gets Colliding

	void SetModelMatrix(matrix4 a_m4ToWorld); //Sets the transform from the local to world matrix
	matrix4 GetModelMatrix(void); //Gets model to world matrix of the sphere

	void RenderBox(vector3 pos); //Renders the box based onthe center in global space

	bool IsColliding(MyBoundingObjectClass* a_other); //Will check the collision with another object

	void SetVisibility(void); //Sets Visibilty

private:
	bool visible; //turn off/on bounding obj 
	bool m_bColliding = false;
	vector3 m_v3CenterLocal = vector3(0.0f); //center  in local space
	vector3 m_v3CenterGlobal = vector3(0.0f); //center  in global space
	matrix4 m_m4ToWorld = IDENTITY_M4; //matrix that takes you from local to global space
	MeshManagerSingleton* m_pMeshMngr = nullptr; //for drawing the obj
};

