#include "AppClass.h"
void Application::InitVariables(void)
{
	// Create all meshes, assign cubes, and places into vector
	for (int i = 0; i < 46; i++)
	{
		MyMesh* mesh = new MyMesh();
		mesh->GenerateCube(1.0f, vector3(0.0f, 0.0f, 0.0f));
		cubeList.push_back(mesh);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	matrix4 m4Model = m4Translate * m4Scale;

	// Rendering every mesh for the invader
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 5, 0, 3));
	cubeList[0]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 5, -1, 3));
	cubeList[1]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 5, -2, 3));
	cubeList[2]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 4, 1, 3));
	cubeList[3]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 4, 0, 3));
	cubeList[4]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, 4, 3));
	cubeList[5]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, 2, 3));
	cubeList[6]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, 1, 3));
	cubeList[7]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, 0, 3));
	cubeList[8]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, -1, 3));
	cubeList[9]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 3, -2, 3));
	cubeList[10]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 2, 3, 3));
	cubeList[11]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 2, 2, 3));
	cubeList[12]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 2, 0, 3));
	cubeList[13]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 2, -1, 3));
	cubeList[14]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value - 2, -3, 3));
	cubeList[15]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value -1, 2, 3));
	cubeList[16]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value -1, 1, 3));
	cubeList[17]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value -1, 0, 3));
	cubeList[18]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value -1, -1, 3));
	cubeList[19]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value -1, -3, 3));
	cubeList[20]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2, 3));
	cubeList[21]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value, 1, 3));
	cubeList[22]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value, 0, 3));
	cubeList[23]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value, -1, 3));
	cubeList[24]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1, 2, 3));
	cubeList[25]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1, 1, 3));
	cubeList[26]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1, 0, 3));
	cubeList[27]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1, -1, 3));
	cubeList[28]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1, -3, 3));
	cubeList[29]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2, 3, 3));
	cubeList[30]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2, 2, 3));
	cubeList[31]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2, 0, 3));
	cubeList[32]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2, -1, 3));
	cubeList[33]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2, -3, 3));

	cubeList[34]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, 4, 3));
	cubeList[35]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, 2, 3));
	cubeList[36]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, 1, 3));
	cubeList[37]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, 0, 3));
	cubeList[38]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, -1, 3));
	cubeList[39]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3, -2, 3));
	cubeList[40]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 4, 1, 3));
	cubeList[41]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 4, 0, 3));
	cubeList[42]->Render(m4Projection, m4View, m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5, 0, 3));
	cubeList[43]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5, -1, 3));
	cubeList[44]->Render(m4Projection, m4View, m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5, -2, 3));
	cubeList[45]->Render(m4Projection, m4View, m4Translate);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);
	
	// Delete all meshes in the vector, then delete the vector
	for (int i = 0; i < cubeList.size(); i++)
	{
		delete cubeList[i];
		cubeList[i] = nullptr;
	}

	//release GUI
	ShutdownGUI();
}