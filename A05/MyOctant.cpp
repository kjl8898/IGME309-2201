#include "MyOctant.h"
using namespace Simplex;
using namespace std;

uint MyOctant::octantCount = 0;
uint MyOctant::maxLevel = 3;
uint MyOctant::idealCount = 5;

MyOctant::MyOctant(uint a_maxLevel, uint a_idealCount)
{
	Init();

	octantCount = 1;
	maxLevel = a_maxLevel;
	idealCount = a_idealCount;

	root = this;

	// Creates a bounding box that encompases all the rigid bodies in the scene
	vector<vector3> boundingBoxBounds;

	for (int i = 0; i < entityManager->GetEntityCount(); i++)
	{
		MyRigidBody* rb = entityManager->GetEntity(i)->GetRigidBody();
		boundingBoxBounds.push_back(rb->GetMinGlobal());
		boundingBoxBounds.push_back(rb->GetMaxGlobal());
	}

	MyRigidBody* rb = new MyRigidBody(boundingBoxBounds);

	vector3 halfWidth = rb->GetHalfWidth();
	float maxLength = halfWidth.x;
	for (int i = 0; i < 3; i++)
	{
		if (maxLength < halfWidth[i])
		{
			maxLength = halfWidth[i];
		}
	}

	vector3 rbCenter = rb->GetCenterLocal();
	center = rbCenter;

	size = maxLength * 2.0f;
	min = center - vector3(maxLength);
	max = center + vector3(maxLength);

	ConstructTree(maxLevel);
}

MyOctant::MyOctant(vector3 a_center, float a_size)
{
	Init();
	octantCount++;

	center = a_center;
	size = a_size;

	min = center - vector3(size) / 2.0f;
	max = center + vector3(size) / 2.0f;
}

MyOctant::MyOctant(MyOctant const& other)
{
	size = other.size;
	id = other.id;
	level = other.id;

	center = other.center;
	min = other.min;
	max = other.max;

	parent = other.parent;
	root = other.root;
	children = other.children;
	childList = other.childList;

	for (int i = 0; i < 8; i++)
	{
		child[i] = other.child[i];
	}
}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}

	return *this;
}

MyOctant::~MyOctant()
{
	Release();
}

float MyOctant::GetSize()
{
	return size;
}

vector3 MyOctant::GetGlobalCenter()
{
	return center;
}

vector3 MyOctant::GetGlobalMin()
{
	return min;
}

vector3 MyOctant::GetGlobalMax()
{
	return max;
}

MyOctant* MyOctant::GetChild(uint a_child)
{
	if (a_child < 0 || a_child > 7)
	{
		return nullptr;
	}
	else
	{
		return child[a_child];
	}
}

MyOctant* MyOctant::GetParent()
{
	return parent;
}

uint MyOctant::GetOctantCount()
{
	return octantCount;
}

void MyOctant::Swap(MyOctant& other)
{
	swap(size, other.size);
	swap(id, other.id);
	swap(level, other.level);

	swap(center, other.center);
	swap(min, other.min);
	swap(max, other.max);

	swap(parent, other.parent);
	swap(root, other.root);
	swap(children, other.children);
	swap(child, other.child);

	for (int i = 0; i < 8; i++)
	{
		swap(child[i], other.child[i]);
	}
}

bool MyOctant::IsColliding(uint a_index)
{
	// Checks this object's bounds against the other object's bounds
	vector3 otherMin = entityManager->GetEntity(a_index)->GetRigidBody()->GetMinGlobal();
	vector3 otherMax = entityManager->GetEntity(a_index)->GetRigidBody()->GetMaxGlobal();

	if (max.x < otherMin.x ||
		min.x > otherMax.x ||
		max.y < otherMin.y ||
		min.y > otherMax.y ||
		max.z < otherMin.z ||
		min.z > otherMax.z)
	{
		return false;
	}

	return true;
}

void MyOctant::Display(uint a_index, vector3 a_color)
{
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center)
		* glm::scale(vector3(size)), a_color, RENDER_WIRE);

	for (int i = 0; i < children; i++)
	{
		child[i]->Display(a_index);
	}
}

void MyOctant::Display(vector3 a_color)
{
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center)
		* glm::scale(vector3(size)), a_color, RENDER_WIRE);

	for (int i = 0; i < children; i++)
	{
		child[i]->Display(a_color);
	}
}

void MyOctant::DisplayLeaves(vector3 a_color)
{
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center)
		* glm::scale(vector3(size)), a_color, RENDER_WIRE);

	int leaves = childList.size();
	for (int i = 0; i < leaves; i++)
	{
		child[i]->DisplayLeaves(a_color);
	}
}

void MyOctant::ClearEntityList()
{
	entityList.clear();

	for (int i = 0; i < children; i++)
	{
		child[i]->ClearEntityList();
	}
}

void MyOctant::Subdivide()
{
	if (level >= maxLevel)
	{
		return;
	}

	if (children != 0)
	{
		return;
	}

	children = 8;
	octantCount += 8;

	float positionIncrement = size / 4.0f;
	float subdivisionLength = size / 2.0f;

	// Bottom Left Back
	vector3 divisionPos = vector3(center.x - positionIncrement, center.y - positionIncrement, center.z - positionIncrement);
	child[0] = new MyOctant(divisionPos, subdivisionLength);

	// Bottom Right Back
	divisionPos.x += subdivisionLength;
	child[1] = new MyOctant(divisionPos, subdivisionLength);

	// Bottom Right Front
	divisionPos.z += subdivisionLength;
	child[2] = new MyOctant(divisionPos, subdivisionLength);

	// Bottom Left Front
	divisionPos.x -= subdivisionLength;
	child[3] = new MyOctant(divisionPos, subdivisionLength);

	// Top Left Front
	divisionPos.y += subdivisionLength;
	child[4] = new MyOctant(divisionPos, subdivisionLength);

	// Top Left Back
	divisionPos.z -= subdivisionLength;
	child[5] = new MyOctant(divisionPos, subdivisionLength);

	// Top Right Back
	divisionPos.x += subdivisionLength;
	child[6] = new MyOctant(divisionPos, subdivisionLength);

	// Top Right Front
	divisionPos.z += subdivisionLength;
	child[7] = new MyOctant(divisionPos, subdivisionLength);

	// Increments the variables in each subdivision. Subdivides any subdivisions with more
	//		than the ideal count.
	for (int i = 0; i < 8; i++)
	{
		child[i]->root = root;
		child[i]->parent = this;
		child[i]->level = level + 1;
		if (child[i]->ContainsMoreThan(idealCount))
		{
			child[i]->Subdivide();
		}
	}
}

bool MyOctant::IsLeaf()
{
	if (children == 0)
	{
		return true;
	}

	return false;
}

bool MyOctant::ContainsMoreThan(uint a_entities)
{
	int numCollisions = 0;
	int totalNumObjects = entityManager->GetEntityCount();

	for (int i = 0; i < totalNumObjects; i++)
	{
		if (IsColliding(i) == true)
		{
			numCollisions++;

			if (numCollisions > a_entities)
			{
				return true;
			}
		}
	}

	return false;
}

void MyOctant::KillBranches()
{
	for (int i = 0; i < children; i++)
	{
		child[i]->KillBranches();
		delete child[i];
		child[i] = nullptr;
	}

	children = 0;
}

void MyOctant::ConstructTree(uint a_maxLevel)
{
	if (level == 0)
	{
		maxLevel = a_maxLevel;

		octantCount = 1;

		entityList.clear();

		KillBranches();
		childList.clear();

		if (ContainsMoreThan(idealCount))
		{
			Subdivide();
		}

		AssignIDtoEntity();

		ConstructList();
	}
}

void MyOctant::AssignIDtoEntity()
{
	for (int i = 0; i < children; i++)
	{
		child[i]->AssignIDtoEntity();
	}

	if (children == 0)
	{
		int totalNumEntities = entityManager->GetEntityCount();
		for (int i = 0; i < totalNumEntities; i++)
		{
			if (IsColliding(i))
			{
				entityList.push_back(i);
				entityManager->AddDimension(i, id);
			}
		}
	}
}

void MyOctant::Release()
{
	KillBranches();
	children = 0;
	size = 0.0f;
	entityList.clear();
	childList.clear();
}

void MyOctant::Init()
{
	id = octantCount;
	size = 0.0f;
	level = 0;
	children = 0;

	center = vector3(0.0f);
	min = vector3(0.0f);
	max = vector3(0.0f);

	meshManager = MeshManager::GetInstance();
	entityManager = MyEntityManager::GetInstance();
}

void MyOctant::ConstructList()
{
	// Adds this object to the childList if it has objects in it
	if (entityList.size() > 0)
	{
		root->childList.push_back(this);
	}

	// Calls ConstructList on all this object's children
	for (int i = 0; i < children; i++)
	{
		child[i]->ConstructList();
	}
}