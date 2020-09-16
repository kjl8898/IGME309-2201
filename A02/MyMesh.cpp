#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// -------------------------------

	float halfHeight = a_fHeight * 0.5f;
	std::vector<vector3> circleVerts;

	float angle = 2 * PI / a_nSubdivisions;

	// Create all verticies for circle
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		circleVerts.push_back(vector3(a_fRadius * sin(i * angle), a_fRadius * cos(i * angle), -halfHeight));
	}

	// Create all triangles for circle and pyramid
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		if (i + 1 == a_nSubdivisions)
		{
			AddTri(circleVerts[i], circleVerts[0], vector3(0, 0, -halfHeight));
			AddTri(circleVerts[0], circleVerts[i], vector3(0, 0, halfHeight));
		}
		else
		{
			AddTri(circleVerts[i], circleVerts[i + 1], vector3(0, 0, -halfHeight));
			AddTri(circleVerts[i + 1], circleVerts[i], vector3(0, 0, halfHeight));
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// -------------------------------

	float halfHeight = a_fHeight * 0.5f;
	std::vector<vector3> circleVertsTop;
	std::vector<vector3> circleVertsBot;

	float angle = 2 * PI / a_nSubdivisions;

	// Create all verticies for circles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		circleVertsTop.push_back(vector3(a_fRadius * sin(i * angle), a_fRadius * cos(i * angle), halfHeight));
		circleVertsBot.push_back(vector3(a_fRadius * sin(i * angle), a_fRadius * cos(i * angle), -halfHeight));
	}

	// Create all triangles for each circle, then connect them with quads
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		if (i + 1 == a_nSubdivisions)
		{
			AddTri(circleVertsTop[0], circleVertsTop[i], vector3(0, 0, halfHeight));
			AddTri(circleVertsBot[i], circleVertsBot[0], vector3(0, 0, -halfHeight));

			AddQuad(circleVertsBot[0], circleVertsBot[i], circleVertsTop[0], circleVertsTop[i]);
		}
		else
		{
			AddTri(circleVertsTop[i + 1], circleVertsTop[i], vector3(0, 0, halfHeight));
			AddTri(circleVertsBot[i], circleVertsBot[i + 1], vector3(0, 0, -halfHeight));

			AddQuad(circleVertsBot[i + 1], circleVertsBot[i], circleVertsTop[i + 1], circleVertsTop[i]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------

	float halfHeight = a_fHeight * 0.5f;
	std::vector<vector3> circleVertsTopIn;
	std::vector<vector3> circleVertsTopOut;
	std::vector<vector3> circleVertsBotIn;
	std::vector<vector3> circleVertsBotOut;

	float angle = 2 * PI / a_nSubdivisions;

	// Create all verticies for circles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		// Top circle
		circleVertsTopIn.push_back(vector3(a_fInnerRadius * sin(i * angle), a_fInnerRadius * cos(i * angle), halfHeight));
		circleVertsTopOut.push_back(vector3(a_fOuterRadius * sin(i * angle), a_fOuterRadius * cos(i * angle), halfHeight));

		// Bottom circle
		circleVertsBotIn.push_back(vector3(a_fInnerRadius * sin(i * angle), a_fInnerRadius * cos(i * angle), -halfHeight));
		circleVertsBotOut.push_back(vector3(a_fOuterRadius * sin(i * angle), a_fOuterRadius * cos(i * angle), -halfHeight));
	}

	// Create all triangles for each circle, then connect them with quads
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		if (i + 1 == a_nSubdivisions)
		{
			// Outer Faces
			AddQuad(circleVertsBotOut[0], circleVertsBotOut[i], circleVertsTopOut[0], circleVertsTopOut[i]);

			// Inner Faces
			AddQuad(circleVertsBotIn[i], circleVertsBotIn[0], circleVertsTopIn[i], circleVertsTopIn[0]);

			// Top Faces
			AddQuad(circleVertsTopOut[0], circleVertsTopOut[i], circleVertsTopIn[0], circleVertsTopIn[i]);

			// Bottom Faces
			AddQuad(circleVertsBotOut[i], circleVertsBotOut[0], circleVertsBotIn[i], circleVertsBotIn[0]);
		}
		else
		{
			// Outer Faces
			AddQuad(circleVertsBotOut[i + 1], circleVertsBotOut[i], circleVertsTopOut[i + 1], circleVertsTopOut[i]);

			// Inner Faces
			AddQuad(circleVertsBotIn[i], circleVertsBotIn[i + 1], circleVertsTopIn[i], circleVertsTopIn[i + 1]);

			// Top Faces
			AddQuad(circleVertsTopOut[i + 1], circleVertsTopOut[i], circleVertsTopIn[i + 1], circleVertsTopIn[i]);

			// Bottom Faces
			AddQuad(circleVertsBotOut[i], circleVertsBotOut[i + 1], circleVertsBotIn[i], circleVertsBotIn[i + 1]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// -------------------------------

	float u = 2 * PI / a_nSubdivisionsB;
	float v = 2 * PI / a_nSubdivisionsA;
	float a = a_fOuterRadius - a_fInnerRadius;
	float c = a_fOuterRadius - (a * 0.5f);

	std::vector<std::vector<vector3>> torusVerts;

	// Generate Points and add them to a 2d vector
	// Torus ring
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		std::vector<vector3> ring;
		// Torus tubes
		for(int j = 0; j < a_nSubdivisionsA; j++)
		{
			ring.push_back(vector3((c + a * cos(v * j)) * cos(u * i),
				((c + a * cos(v * j)) * sin(u * i)),
					a * sin(v * j)));
		}
		torusVerts.push_back(ring);
	}

	// Draw quads between the points
	for (int i = 0; i < a_nSubdivisionsB; i++)
	{
		// Points on ring
		for (int j = 0; j < a_nSubdivisionsA; j++)
		{
			// Last ring
			if (i + 1 == a_nSubdivisionsB && j + 1 < a_nSubdivisionsA)
			{
				AddQuad(torusVerts[i][j + 1], torusVerts[i][j], torusVerts[0][j + 1], torusVerts[0][j]);
			}

			// Any point
			else if (i + 1 < a_nSubdivisionsB && j + 1 < a_nSubdivisionsA)
			{
				AddQuad(torusVerts[i][j + 1], torusVerts[i][j], torusVerts[i + 1][j + 1], torusVerts[i + 1][j]);
			}

			// Last point on ring
			else if (j + 1 == a_nSubdivisionsA && i + 1 < a_nSubdivisionsB)
			{
				AddQuad(torusVerts[i][0], torusVerts[i][j], torusVerts[i + 1][0], torusVerts[i + 1][j]);
			}

			// Last point and last ring
			else
			{
				AddQuad(torusVerts[i][0], torusVerts[i][j], torusVerts[0][0], torusVerts[0][j]);
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// -------------------------------

	// Sphere was tested with 10 subdivisons
	//a_nSubdivisions = 10;

	float longitude = 2 * PI / a_nSubdivisions;
	float colatitude = PI / a_nSubdivisions;
	float r = a_fRadius;

	std::vector<std::vector<vector3>> sphereVerts;

	// Create points for the sphere and add them to the 2d vector
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		std::vector<vector3> circle;
		{
			for (int j = 0; j < a_nSubdivisions; j++)
			{
				circle.push_back(vector3(r * cos(longitude * i) * sin(colatitude * j),
					r * sin(longitude * i) * sin(colatitude * j),
					r * cos(colatitude * j)));
			}
		}
		sphereVerts.push_back(circle);
	}

	// Generate the surfaces
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// Last circle
			if (i + 1 == a_nSubdivisions && j + 1 < a_nSubdivisions)
			{
				AddQuad(sphereVerts[i][j + 1], sphereVerts[i][j], sphereVerts[0][j + 1], sphereVerts[0][j]);
			}

			// Any Point
			else if (i + 1 < a_nSubdivisions && j + 1 < a_nSubdivisions)
			{
				AddQuad(sphereVerts[i][j + 1], sphereVerts[i][j], sphereVerts[i + 1][j + 1], sphereVerts[i + 1][j]);
			}

			// Endcap
			else if (i + 1 < a_nSubdivisions && j + 1 == a_nSubdivisions)
			{
				AddTri(vector3(0, 0, -r), sphereVerts[i][j], sphereVerts[i + 1][j]);
			}

			// Final Endcap Piece
			else
			{
				AddTri(vector3(0, 0, -r), sphereVerts[i][j], sphereVerts[0][j]);
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}