#include "AppClass.h"
void AppClass::ProcessKeyboard(void)
{
	bool bModifier = false;
	float fSpeed = 0.01f;

#pragma region ON_KEY_PRESS_RELEASE
	static bool	bLastF1 = false, bLastF2 = false, bLastF3 = false, bLastF4 = false, bLastF5 = false,
		bLastF6 = false, bLastF7 = false, bLastF8 = false, bLastF9 = false, bLastF10 = false,
		bLastEscape = false, bLastF = false, bLastH = false, bLastB = false, bLastK = false;
#define ON_KEY_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion

#pragma region Modifiers
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		bModifier = true;
#pragma endregion

#pragma region Camera Positioning
	if(bModifier)
		fSpeed *= 10.0f;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCameraMngr->MoveForward(fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCameraMngr->MoveForward(-fSpeed);
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCameraMngr->MoveSideways(-fSpeed);

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCameraMngr->MoveSideways(fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_pCameraMngr->MoveVertical(-fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_pCameraMngr->MoveVertical(fSpeed);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_v3Position += vector3(-0.1f, 0.0f, 0.0f);
		m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_v3Position += vector3(0.1f, 0.0f, 0.0f);
		m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!bModifier)
		{
			m_v3Position += vector3(0.0f, 0.1f, 0.0f);
			m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
		}
		else
		{
			m_v3Position += vector3(0.0f, 0.0f,-0.1f);
			m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (!bModifier)
		{
			m_v3Position += vector3(0.0f, -0.1f, 0.0f);
			m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
		}
		else
		{
			m_v3Position += vector3(0.0f, 0.0f, 0.1f);
			m_bObjManager->objectList[0].SetModelMatrix(glm::translate(m_v3Position));
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
		if (!bModifier)
		{
			//turn on and off visibility
			m_bObjManager->SetGeneralVisibility(false);
		}
		else {
			m_bObjManager->SetGeneralVisibility(true);
		}
	}

	

	static bool bSOVis = false;
	static bool bSOCheck = false;
	static bool bBOVis = false;

	//turn on/off SO visibilty
	ON_KEY_PRESS_RELEASE(H, NULL, bSOVis = true);

	if (bSOVis) {
		m_bObjManager->octree.SetSOVisibility(!m_bObjManager->octree.GetSOVisibility());
		bSOVis = false;
	}

	//turn on/off SO checking
	ON_KEY_PRESS_RELEASE(K, NULL, bSOCheck = true);

	if (bSOCheck) {
		m_bObjManager->octree.SetSOCheck(!m_bObjManager->octree.GetSOCheck());
		bSOCheck = false;
	}

	ON_KEY_PRESS_RELEASE(B, NULL, bBOVis = true);

	if (bBOVis) {
		m_bObjManager->SetAABBVisibility(!m_bObjManager->GetAABBVisibility());
		bBOVis = false;
	}

#pragma endregion

#pragma region Other Actions
	ON_KEY_PRESS_RELEASE(Escape, NULL, PostMessage(m_pWindow->GetHandler(), WM_QUIT, NULL, NULL));
	ON_KEY_PRESS_RELEASE(F1, NULL, m_pCameraMngr->SetCameraMode(CAMPERSP));
	ON_KEY_PRESS_RELEASE(F2, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOZ));
	ON_KEY_PRESS_RELEASE(F3, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOY));
	ON_KEY_PRESS_RELEASE(F4, NULL, m_pCameraMngr->SetCameraMode(CAMROTHOX));
	static bool bFPSControll = false;
	ON_KEY_PRESS_RELEASE(F, bFPSControll = !bFPSControll, m_pCameraMngr->SetFPS(bFPSControll));
#pragma endregion
}
void AppClass::ProcessMouse(void)
{
	m_bArcBall = false;
	m_bFPC = false;
#pragma region ON_MOUSE_PRESS_RELEASE
	static bool	bLastLeft = false, bLastMiddle = false, bLastRight = false;
#define ON_MOUSE_PRESS_RELEASE(key, pressed_action, released_action){  \
			bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::key);			\
			if(pressed){											\
				if(!bLast##key) pressed_action;}/*Just pressed? */\
			else if(bLast##key) released_action;/*Just released?*/\
			bLast##key = pressed; } //remember the state
#pragma endregion
	bool bLeft = false;
	ON_MOUSE_PRESS_RELEASE(Left, NULL, bLeft = true)
	if (bLeft)
	{
		//Turn off the visibility of all BOs for all instances
		m_pMeshMngr->SetVisibleBO(BD_NONE, "ALL", -1);
		//Get the Position and direction of the click on the screen
		std::pair<vector3, vector3> pair =
			m_pCameraMngr->GetClickAndDirectionOnWorldSpace(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		float fDistance = 0;//Stores the distance to the first colliding object
		m_selection = m_pMeshMngr->IsColliding(pair.first, pair.second, fDistance);

		//If there is a collision
		if (m_selection.first >= 0)
		{
			//Turn on the BO of the group
			m_pMeshMngr->SetVisibleBO(BD_OB, m_selection.first, m_selection.second);

			//Turn of the BO of the instance but not the group
			m_pMeshMngr->SetVisibleBO(BD_NONE, m_selection.first, -2);
		}
	}
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		m_bArcBall = true;
	
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_bFPC = true;
}
