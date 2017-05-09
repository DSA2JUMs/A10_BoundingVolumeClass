#include "AppClass.h"

// Necessary for setting the instance as a nullptr to begin with
BoundingObjectManagerSingleton* BoundingObjectManagerSingleton::instance = nullptr;

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("A10_BoundingVolumeClass");
}

void AppClass::InitVariables(void)
{
	// Get the instance of the BoundingObjectManagerSingleton
	m_bObjManager = BoundingObjectManagerSingleton::GetInstance();

	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up

	//Load a model onto the Mesh manager

	//creating bounding spheres for both models
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie");
	m_bObjManager->CreateBoundingObject(m_pMeshMngr->GetVertexList("Zombie"));
	m_bObjManager->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Zombie"), 0);

	matrix4 m4Position = glm::translate(vector3(3.0f, 0.0f, 0.0f));

	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_bObjManager->CreateBoundingObject(m_pMeshMngr->GetVertexList("Steve"));
	m_pMeshMngr->SetModelMatrix(m4Position, "Steve");
	m_bObjManager->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"), 1);

	matrix4 m4Position2 = glm::translate(vector3(3.5f, 1.5f, 0.0f));

	m_pMeshMngr->LoadModel("Minecraft\\Cow.obj", "Cow");
	m_bObjManager->CreateBoundingObject(m_pMeshMngr->GetVertexList("Cow"));
	m_pMeshMngr->SetModelMatrix(m4Position2, "Cow");
	m_bObjManager->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Cow"), 2);

	//m_bObjManager->SetColor(REBLUE);
	//m_bObjManager->SetSphereColor(RECYAN);

	//add companion cubes in a sphere
	int counter = 3;
	m_nInstances = 200;
	int nSquare = static_cast<int>(std::sqrt(m_nInstances));
	m_nInstances = nSquare * nSquare;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			String sInstance = "Cube_" + std::to_string(i) + "_" + std::to_string(j);
			matrix4 m4Positions = glm::translate(static_cast<float>(i - nSquare / 2.0f), static_cast<float>(j), 0.0f);
			m4Positions = glm::translate(vector3(glm::sphericalRand(20.0f)));
			m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", sInstance, false, m4Positions);
			m_bObjManager->CreateBoundingObject(m_pMeshMngr->GetVertexList(sInstance)); // renders each companion cube
			m_bObjManager->SetModelMatrix(m4Positions, counter); // renders each BO 
			counter++;
		}
	}
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	//Object Movement
	static float fTimer = 0.0f;
	static int nClock = m_pSystem->GenClock();
	float fDeltaTime = static_cast<float>(m_pSystem->LapClock(nClock));
	fTimer += fDeltaTime;
	static vector3 v3Start = vector3(3.0, 0.0, 0.0);
	static vector3 v3End = vector3(10.0, 0.0, 0.0);
	float fPercentage = MapValue(fTimer, 0.0f, 3.0f, 0.0f, 1.0f);
	vector3 v3Current = glm::lerp(v3Start, v3End, fPercentage);
	matrix4 mTranslation = glm::translate(v3Current);

	//set the translate to create the transform matrix
	matrix4 m4Transform = glm::translate(m_v3Position) * ToMatrix4(m_qArcBall);
	m_pMeshMngr->SetModelMatrix(m4Transform, "Zombie"); //set the matrix to the model
		
	m_pMeshMngr->SetModelMatrix(mTranslation, "Steve");

	m_bObjManager->RenderBoundingObject();
	m_bObjManager->RenderOctree();
	
	m_bObjManager->CheckCollisions();

	if (fPercentage > 1.0f)
	{
		fTimer = 0.0f;
		std::swap(v3Start, v3End);
	}

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("<K> SO Check: ");
	if(m_bObjManager->octree.GetSOCheck())
		m_pMeshMngr->PrintLine("true");
	else
		m_pMeshMngr->PrintLine("false");
	m_pMeshMngr->PrintLine("<H> Display Octree ");
	m_pMeshMngr->PrintLine("<B> Display BO ");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//m_pMeshMngr->Print("Center: (");
	//m_pMeshMngr->Print(std::to_string(m_bObjManager->objectList[0].GetCenterGlobal().x), RERED);
	//m_pMeshMngr->Print(" , ");
	//m_pMeshMngr->Print(std::to_string(m_bObjManager->objectList[0].GetCenterGlobal().y), RERED);
	//m_pMeshMngr->Print(" , ");
	//m_pMeshMngr->Print(std::to_string(m_bObjManager->objectList[0].GetCenterGlobal().z), RERED);
	//m_pMeshMngr->PrintLine(")");

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}