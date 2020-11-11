#pragma once

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		static uint octantCount;
		static uint maxLevel;
		static uint idealCount;

		uint id = 0;
		uint level = 0;
		uint children = 0;

		float size = 0.0f;

		MeshManager* meshManager = nullptr;
		MyEntityManager* entityManager = nullptr;

		vector3 center = vector3(0.0f);
		vector3 min = vector3(0.0f);
		vector3 max = vector3(0.0f);

		MyOctant* parent = nullptr;
		MyOctant* child[8];

		std::vector<uint> entityList;

		MyOctant* root = nullptr;
		std::vector<MyOctant*> childList;

	public:

		// Constructor
		MyOctant(uint a_maxLevel = 2, uint a_idealCount = 5);

		// Constructor
		//		
		MyOctant(vector3 a_center, float a_size);

		// Copy Constructor
		MyOctant(MyOctant const& other);

		// Copy Assignment Operator
		MyOctant& operator=(MyOctant const& other);

		// Destructor
		~MyOctant();

		// Get Size method
		//		Returns the size of the octant
		float GetSize();

		// Get Global Center method
		//		Returns the center of the octant in global space
		vector3 GetGlobalCenter();

		// Get Global Min method
		//		Returns the minimum corner of the octant in global space
		vector3 GetGlobalMin();

		// Get Global Max method
		//		Returns the maximum corner of the octant in global space
		vector3 GetGlobalMax();

		// Get Child method
		//		Returns the child of the specified index
		MyOctant* GetChild(uint a_child);

		// Get Parent method
		//		Returns the parent node of the octant
		MyOctant* GetParent();

		// Get Octant Count method
		//		Gets the total number of octants in the scene
		uint GetOctantCount();

		// Swap method
		//		Swaps with objects contents for another object's contents
		void Swap(MyOctant& other);

		// Is Colliding method
		//		Returns if two objects are colliding
		bool IsColliding(uint a_index);

		// Display method
		//		Displays by index
		void Display(uint a_index, vector3 a_color = C_YELLOW);

		// Display method
		//		Desplays the whole volume of the octant
		void Display(vector3 a_color = C_YELLOW);

		// Display Leaves method
		//		Displays any leaves that have data
		void DisplayLeaves(vector3 a_color = C_YELLOW);

		// Clears the entity list of each node
		void ClearEntityList();

		// Subdivide method
		//		Allocates 8 smaller octants inside the child
		void Subdivide();

		// Is Leaf method
		//		Returns if the octant is a leaf or not
		bool IsLeaf();

		// Contains More Than method
		//		Returns if the octant contains more objects than specified
		bool ContainsMoreThan(uint a_entities);

		// Kill Branches method
		//		Deletes all child nodes of the octant
		void KillBranches();

		// Construct Tree method
		//		Creats a tree using subdivisions
		void ConstructTree(uint a_maxLevel = 3);

		// Assign ID to Entity method
		//		Traverse the tree up to the leaves and sets the objects in them to the index
		void AssignIDtoEntity();


	private:

		// Release method
		//		Deallocates the memory
		void Release();

		// Init method
		//		Allocates the memory
		void Init();

		// Construct List method
		//		Creates the list of all leaves that contain objects
		void ConstructList();
	};
}