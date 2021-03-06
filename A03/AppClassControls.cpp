#include "AppClass.h"
using namespace Simplex;
//Mouse
void Application::ProcessMouseMovement(sf::Event a_event)
{
	//get global mouse position
	sf::Vector2i mouse = sf::Mouse::getPosition();
	sf::Vector2i window = m_pWindow->getPosition();
	m_v3Mouse.x = static_cast<float>(mouse.x - window.x);
	m_v3Mouse.y = static_cast<float>(mouse.y - window.y);
	if(!m_pSystem->IsWindowFullscreen() && !m_pSystem->IsWindowBorderless())
		m_v3Mouse += vector3(-8.0f, -32.0f, 0.0f);
	gui.io.MousePos = ImVec2(m_v3Mouse.x, m_v3Mouse.y);
}
void Application::ProcessMousePressed(sf::Event a_event)
{
	switch (a_event.mouseButton.button)
	{
	default: break;
	case sf::Mouse::Button::Left:
		gui.m_bMousePressed[0] = true;
		break;
	case sf::Mouse::Button::Middle:
		gui.m_bMousePressed[1] = true;
		m_bArcBall = true;
		break;
	case sf::Mouse::Button::Right:
		gui.m_bMousePressed[2] = true;
		m_bFPC = true;
		break;
	}

	for (int i = 0; i < 3; i++)
		gui.io.MouseDown[i] = gui.m_bMousePressed[i];
}
void Application::ProcessMouseReleased(sf::Event a_event)
{
	switch (a_event.mouseButton.button)
	{
	default: break;
	case sf::Mouse::Button::Left:
		gui.m_bMousePressed[0] = false;
		break;
	case sf::Mouse::Button::Middle:
		gui.m_bMousePressed[1] = false;
		m_bArcBall = false;
		break;
	case sf::Mouse::Button::Right:
		gui.m_bMousePressed[2] = false;
		m_bFPC = false;
		break;
	}

	for (int i = 0; i < 3; i++)
		gui.io.MouseDown[i] = gui.m_bMousePressed[i];
}
void Application::ProcessMouseScroll(sf::Event a_event)
{
	gui.io.MouseWheel = a_event.mouseWheelScroll.delta;
	float fSpeed = a_event.mouseWheelScroll.delta;
	float fMultiplier = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

	if (fMultiplier)
		fSpeed *= 2.0f;
}
//Keyboard
void Application::ProcessKeyPressed(sf::Event a_event)
{
	switch (a_event.key.code)
	{
	default: break;
	case sf::Keyboard::Space:
		break;
	}
	//gui
	gui.io.KeysDown[a_event.key.code] = true;
	gui.io.KeyCtrl = a_event.key.control;
	gui.io.KeyShift = a_event.key.shift;
}
void Application::ProcessKeyReleased(sf::Event a_event)
{
	static bool bFPSControl = false;

	switch (a_event.key.code)
	{
	default: break;
	case sf::Keyboard::Escape:
		m_bRunning = false;
		break;
	case sf::Keyboard::F1:
		m_pCamera->SetPerspective();
		m_pCamera->CalculateProjectionMatrix();
		break;
	case sf::Keyboard::F2:
		m_pCamera->SetPerspective(false);
		m_pCamera->CalculateProjectionMatrix();
		break;
	case sf::Keyboard::Add:
		++m_uActCont;
		m_uActCont %= 8;
		if (m_uControllerCount > 0)
		{
			while (m_pController[m_uActCont]->uModel == SimplexController_NONE)
			{
				++m_uActCont;
				m_uActCont %= 8;
			}
		}
		break;
	case sf::Keyboard::Subtract:
		--m_uActCont;
		if (m_uActCont > 7)
			m_uActCont = 7;
		if (m_uControllerCount > 0)
		{
			while (m_pController[m_uActCont]->uModel == SimplexController_NONE)
			{
				--m_uActCont;
				if (m_uActCont > 7)
					m_uActCont = 7;
			}
		}
		break;
	}

	//gui
	gui.io.KeysDown[a_event.key.code] = false;
	gui.io.KeyCtrl = a_event.key.control;
	gui.io.KeyShift = a_event.key.shift;
}
//Joystick
void Application::ProcessJoystickConnected(uint nController)
{
	// if the index of the controller is larger than 7
	if (nController > 7)
		return;

	bool bConnected = sf::Joystick::isConnected(nController);
	if (bConnected)
	{
		SafeDelete(m_pController[nController]);
		sf::Joystick::Identification joyID = sf::Joystick::getIdentification(nController);
		String productName = joyID.name.toAnsiString();
		int nVendorID = joyID.vendorId;
		int nProductID = joyID.productId;
		m_pController[nController] = new ControllerInput(nVendorID, nProductID);
		//m_pWindow->setJoystickThreshold(7);
		++m_uControllerCount;
	}
}
void Application::ProcessJoystickPressed(sf::Event a_event)
{
	//Check the ID of the controller
	int nID = a_event.joystickButton.joystickId;

	//Identify the button pressed
	int nButton = a_event.joystickButton.button;
	//map the value to our control value
	m_pController[nID]->button[m_pController[nID]->mapButton[nButton]] = true;

	//Process...

	//If we press L3 + R3 on the active controller we quit the application
	if (m_pController[m_uActCont]->button[SimplexKey_L3] && m_pController[m_uActCont]->button[SimplexKey_R3])
		m_bRunning = false;
}
void Application::ProcessJoystickReleased(sf::Event a_event)
{
	//Check the ID of the controller
	int nID = a_event.joystickButton.joystickId;

	//Identify the button pressed
	int nButton = a_event.joystickButton.button;
	//map the value to our control value
	m_pController[nID]->button[m_pController[nID]->mapButton[nButton]] = false;

	//Process...

	//if we released the Pad key on the active controller quit the application
	if (m_pController[m_uActCont]->mapButton[nButton] == SimplexKey_Pad)
		m_bRunning = false;
}
void Application::ProcessJoystickMoved(sf::Event a_event)
{
	int nID = a_event.joystickMove.joystickId;
	float fPosition = a_event.joystickMove.position;
	int nAxis = a_event.joystickMove.axis;

	//invert vertical axis for sticks
	if (nAxis == sf::Joystick::Axis::Y || nAxis == sf::Joystick::Axis::R)
	{
		fPosition *= -1.0f;
	}

	//Adjust with threshold
	float fThreshold = 10.0f;
	if (nAxis != sf::Joystick::Axis::PovX || nAxis != sf::Joystick::Axis::PovY)
	{
		if (fPosition > -fThreshold && fPosition < fThreshold)
		{
			fPosition = 0.0f;
		}
	}

	/*
	SFML does not use XInput so XBOX one controller is a special snowflake,
	LTrigger and RTrigger share the same axis (sf::Joystick::Axis::Z)
	instead of separate ones as	in other controllers:
	LTrigger reports from 0 to 100 and RTrigger reports from 0 to -100
	I mapped the values from 0 to 100 in both cases to keep consistent
	with other controllers, but sharing the axis means that triggers cannot
	be independent from each other
	*/
	//PS4 Controller
	if (m_pController[nID]->uModel == SimplexController_DualShock4)
	{
		//For Axis U or V
		if (nAxis == sf::Joystick::Axis::U || nAxis == sf::Joystick::Axis::V)
		{
			fPosition = MapValue(fPosition, -100.0f, 100.0f, 0.0f, 100.0f);
		}
		m_pController[nID]->axis[m_pController[nID]->mapAxis[nAxis]] = fPosition;
	}
	//Nintendo Switch Pro
	else if (m_pController[nID]->uModel == SimplexController_SwitchPro)
	{
		if (nAxis != sf::Joystick::Axis::PovX && nAxis != sf::Joystick::Axis::PovY)
		{
			fPosition = MapValue(fPosition, -75.0f, 75.0f, -100.0f, 100.0f);
			if (fPosition > 100) fPosition = 100;
			if (fPosition < -100) fPosition = -100;
			if (fPosition > -10 && fPosition < 10) fPosition = 0;
		}
		m_pController[nID]->axis[m_pController[nID]->mapAxis[nAxis]] = fPosition;
	}
	//Microsoft controllers
	else if (m_pController[nID]->uModel == SimplexController_XBONE || m_pController[nID]->uModel == SimplexController_360)
	{
		//Axis different than Z
		if (nAxis != sf::Joystick::Axis::Z)
		{
			m_pController[nID]->axis[m_pController[nID]->mapAxis[nAxis]] = fPosition;
		}
		else //Z axis
		{
			//get the uncleaned value
			fPosition = a_event.joystickMove.position;
			//positive values go on the left trigger
			if (fPosition >= 0)
			{
				if (fPosition < 25.0f)
					fPosition = 0.0f;
				m_pController[nID]->axis[SimplexAxis_L] = fPosition;
			}
			//negative values go on the right trigger
			else
			{
				if (fPosition > -25.0f)
					fPosition = 0.0f;
				m_pController[nID]->axis[SimplexAxis_R] = -fPosition;
			}
		}
	}
	//8Bitdo NES30 PRO
	else if (m_pController[nID]->uModel == SimplexController_NES30PRO)
	{
		m_pController[nID]->axis[m_pController[nID]->mapAxis[nAxis]] = fPosition;
	}
	//Other controllers
	else
	{
		m_pController[nID]->axis[m_pController[nID]->mapAxis[nAxis]] = fPosition;
	}
}
/*
Continuous update (once per frame) for discreet input use
process events.
*/
//Mouse
void Application::ArcBall(float a_fSensitivity)
{
	//If the arcball is not enabled return
	if (!m_bArcBall)
		return;

	//static quaternion qArcBall;
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in position and update the quaternion orientation based on it
	float DeltaMouse;
	if (MouseX < CenterX)
	{
		DeltaMouse = static_cast<float>(CenterX - MouseX);
		m_qArcBall = quaternion(vector3(0.0f, glm::radians(a_fSensitivity * DeltaMouse), 0.0f)) * m_qArcBall;
	}
	else if (MouseX > CenterX)
	{
		DeltaMouse = static_cast<float>(MouseX - CenterX);
		m_qArcBall = quaternion(vector3(0.0f, glm::radians(-a_fSensitivity * DeltaMouse), 0.0f)) * m_qArcBall;
	}

	if (MouseY < CenterY)
	{
		DeltaMouse = static_cast<float>(CenterY - MouseY);
		m_qArcBall = quaternion(vector3(glm::radians(-a_fSensitivity * DeltaMouse), 0.0f, 0.0f)) * m_qArcBall;
	}
	else if (MouseY > CenterY)
	{
		DeltaMouse = static_cast<float>(MouseY - CenterY);
		m_qArcBall = quaternion(vector3(glm::radians(a_fSensitivity * DeltaMouse), 0.0f, 0.0f)) * m_qArcBall;
	}

	SetCursorPos(CenterX, CenterY);//Position the mouse in the center
								   //return qArcBall; // return the new quaternion orientation
}
void Application::CameraRotation(float a_fSpeed)
{
	if (m_bFPC == false)
		return;

	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (MouseX < CenterX)
	{
		fDeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += fDeltaMouse * a_fSpeed;
	}
	else if (MouseX > CenterX)
	{
		fDeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= fDeltaMouse * a_fSpeed;
	}

	if (MouseY < CenterY)
	{
		fDeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= fDeltaMouse * a_fSpeed;
	}
	else if (MouseY > CenterY)
	{
		fDeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += fDeltaMouse * a_fSpeed;
	}
	//Change the Yaw and the Pitch of the camera




	// I tried a lot of different combinations of to try and solve the Gimbal lock...

	// This section was an attempt at applying E08's quaternion math to the camera


	//matrix4 m_m4CameraOrientation;

	//matrix4 m4OrientX = glm::rotate(IDENTITY_M4, glm::radians(-fAngleX), vector3(1.0f, 0.0f, 0.0f));
	//matrix4 m4OrientY = glm::rotate(IDENTITY_M4, glm::radians(fAngleY), vector3(0.0f, 1.0f, 0.0f));
	//matrix4 m4OrientZ = glm::rotate(IDENTITY_M4, glm::radians(forward.z), vector3(0.0f, 0.0f, 1.0f));

	//matrix4 m4Orientation = m4OrientX * m4OrientY;
	//m_m4CameraOrientation = glm::toMat4(m4Orientation);

	//forward += vector3(cos(fAngleY), sin(-fAngleX), 0);



	// This section was an attempt to create a matrix representing the camera's orientation, then apply rotations to it


	//quaternion q1;
	//quaternion qX;
	//quaternion qY;
	
	// Multiple applications of trying to get the camera's orientation

	//vector3 cameraPosition = m_pCamera->GetPosition();
	//vector3 cameraTarget = m_pCamera->GetTarget();
	//vector3 cameraAbove = m_pCamera->GetAbove();
	//q1 = m_pCamera->GetTarget() - m_pCamera->GetPosition();

	//vector3 forward = m_pCamera->GetTarget() - m_pCamera->GetPosition();
	//vector3 right = glm::cross(m_pCamera->GetTarget() - m_pCamera->GetPosition(), m_pCamera->GetAbove() - m_pCamera->GetPosition());
	//vector3 up = glm::cross(forward, right);

	//vector3 cameraForward = glm::normalize(cameraTarget - cameraPosition);
	//vector3 cameraRight = glm::normalize(glm::cross(cameraAbove, cameraForward));
	//vector3 cameraUp = glm::cross(cameraForward, cameraRight);

	//quaternion cameraOrientation = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);

	//q1 = glm::normalize(forward) * glm::normalize(right) * glm::normalize(up);


	// Multiple attempts to apply rotations to the differnt axes

	//qX = glm::angleAxis(glm::radians(-fAngleX), glm::normalize(glm::cross(m_pCamera->GetTarget() - m_pCamera->GetPosition(), m_pCamera->GetAbove() - m_pCamera->GetPosition())));
	//qY = glm::angleAxis(glm::radians(fAngleY), glm::normalize(m_pCamera->GetAbove() - m_pCamera->GetPosition()));

	//qX = glm::angleAxis(glm::radians(-fAngleX), m_pCamera->GetTarget() + vector3(1.0f, 0.0f, 0.0f));
	//qY = glm::angleAxis(glm::radians(fAngleY), m_pCamera->GetTarget() + vector3(0.0f, 1.0f, 0.0f));

	//m_pCamera->SetTarget(vector3(fAngleY, fAngleX, 0.0f) - m_pCamera->GetPosition());

	//qX = glm::rotate(IDENTITY_M4, glm::radians(-fAngleX), vector3(1.0f, 0.0f, 0.0f));
	//qX = (quaternion((cos(-fAngleX) + sin(fAngleX)), forward.x, forward.y, forward.z));
	//qY = ((cos(fAngleY) + sin(fAngleY) * forward));
	

	// Attempting to apply the rotation matricies to the orientation matrix

	//qY = glm::rotate(IDENTITY_M4, glm::radians(fAngleY), vector3(0.0f, 1.0f, 0.0f));

	//forward = forward * qX * qY;

	//cameraOrientation = cameraOrientation * qX * qY;

	//q1 = qX * qY;

	//cameraOrientation = q1;

	//m4Orientation = m4Orientation * qX * qY;

	//cameraForward = cameraForward * cameraOrientation;




	// Creates a point in front of the camera using cross products rotated by the above angles.
	//		Causes a Gimbal lock
	vector3 forwardPoint = m_pCamera->GetTarget();
	forwardPoint += glm::normalize(glm::cross(AXIS_Y, m_pCamera->GetTarget() - m_pCamera->GetPosition())) * (fAngleY);
	forwardPoint += glm::normalize(glm::cross(AXIS_X, m_pCamera->GetTarget() - m_pCamera->GetPosition())) * (-fAngleX);

	m_pCamera->SetTarget(forwardPoint);


	SetCursorPos(CenterX, CenterY);//Position the mouse in the center
}
//Keyboard
void Application::ProcessKeyboard(void)
{
	/*
	This is used for things that are continuously happening,
	for discreet on/off use ProcessKeyboardPressed/Released
	*/
#pragma region Camera Position
	float fSpeed = 0.1f;
	float fMultiplier = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

	if (fMultiplier)
		fSpeed *= 5.0f;

	// Calculate camera movements
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_pCamera->MoveForward(fSpeed);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_pCamera->MoveForward(-fSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_pCamera->MoveSideways(fSpeed);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_pCamera->MoveSideways(-fSpeed);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_pCamera->MoveVertical(-fSpeed);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_pCamera->MoveVertical(fSpeed);

#pragma endregion
}
//Joystick
void Application::ProcessJoystick(void)
{
	/*
	This is used for things that are continuously happening,
	for discreet on/off use ProcessJoystickPressed/Released
	*/
#pragma region Camera Position
	float fForwardSpeed = m_pController[m_uActCont]->axis[SimplexAxis_Y] / 150.0f;
	float fHorizontalSpeed = m_pController[m_uActCont]->axis[SimplexAxis_X] / 150.0f;
	float fVerticalSpeed = m_pController[m_uActCont]->axis[SimplexAxis_R] / 150.0f -
		m_pController[m_uActCont]->axis[SimplexAxis_L] / 150.0f;

	bool fMultiplier = m_pController[m_uActCont]->button[SimplexKey_L1] ||
		m_pController[m_uActCont]->button[SimplexKey_R1];

	if (fMultiplier)
	{
		fForwardSpeed *= 3.0f;
		fHorizontalSpeed *= 3.0f;
		fVerticalSpeed *= 3.0f;
	}

	m_pCameraMngr->MoveForward(fForwardSpeed);
	m_pCameraMngr->MoveSideways(fHorizontalSpeed);
	m_pCameraMngr->MoveVertical(fVerticalSpeed);
#pragma endregion
#pragma region Camera Orientation
	//Change the Yaw and the Pitch of the camera
	float fUSpeed = glm::radians(m_pController[m_uActCont]->axis[SimplexAxis_U] / 150.0f);
	float fVSpeed = glm::radians(m_pController[m_uActCont]->axis[SimplexAxis_V] / 150.0f);
	m_pCameraMngr->ChangeYaw(-fUSpeed);
	m_pCameraMngr->ChangePitch(fVSpeed);
#pragma endregion
#pragma region ModelOrientation Orientation
	m_qArcBall = quaternion(vector3(glm::radians(m_pController[m_uActCont]->axis[SimplexAxis_POVY] / 20.0f), 0.0f, 0.0f)) * m_qArcBall;
	if (fMultiplier)
	{
		m_qArcBall = quaternion(vector3(0.0f, 0.0f, glm::radians(m_pController[m_uActCont]->axis[SimplexAxis_POVX] / 20.0f))) * m_qArcBall;
	}
	else
	{
		m_qArcBall = quaternion(vector3(0.0f, glm::radians(m_pController[m_uActCont]->axis[SimplexAxis_POVX] / 20.0f), 0.0f)) * m_qArcBall;
	}
#pragma endregion
}