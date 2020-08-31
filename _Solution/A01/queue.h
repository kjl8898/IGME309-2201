#ifndef _QUEUE_H
#define _QUEUE_H

#include <iostream>

#endif

using namespace std;

template <class Placeholder>

class PriorityQueue
{
public:

	// Default Constructor
	PriorityQueue()
	{
		queueSize = 1;
		numItems = 0;
		queueArray = new Placeholder[queueSize];
	}

	// Copy Constructor
	//		Creates a new Priority Queue that is a copy of
	//		an existing queue
	PriorityQueue(const PriorityQueue& other)
	{
		queueSize = other.queueSize;
		numItems = other.numItems;

		queueArray = new Placeholder[queueSize];

		copy(other.queueArray, other.queueArray + other.queueSize, queueArray);
	}

	// Copy Assignment Operator
	//		Overrides the copy assignment operator to copy
	//		all required elements
	PriorityQueue& operator=(const PriorityQueue& other)
	{
		if (this == &other)
		{
			return *this;
		}

		// Deletes old queue data
		if (queueArray != nullptr)
		{
			delete[] queueArray;
			queueArray = nullptr;

			queueSize = 0;
			numItems = 0;
		}

		// Sets the new queue's data
		queueSize = other.queueSize;
		numItems = other.numItems;

		queueArray = new Placeholder[queueSize];

		copy(other.queueArray, other.queueArray + other.queueSize, queueArray);

		return *this;
	}

	// Deconstructor
	~PriorityQueue()
	{
		delete[] queueArray;
		queueArray = nullptr;

		queueSize = 0;
		numItems = 0;
	}

	// Push Method
	//		
	void Push(Placeholder item)
	{
		// Creates a larger queue of double the size if needed
		if (numItems + 1 > queueSize)
		{
			queueSize *= 2;
			Placeholder* tempQueueArray = new Placeholder[queueSize];

			for (int i = 0; i < queueSize; i++)
			{
				tempQueueArray[i] = NULL;
			}

			// Copying the old array into the new array
			for (int i = 0; i < numItems; i++)
			{
				tempQueueArray[i] = queueArray[i];
			}

			delete[] queueArray;
			queueArray = tempQueueArray;
		}

		if (numItems == 0)
		{
			queueArray[0] = item;
			numItems++;
		}
		else
		{
			// Finds where the item belongs in the queue
			int queueLoc = 0;
			while (item >= queueArray[queueLoc] && queueLoc < numItems)
			{
				queueLoc++;
			}

			// Shifts all larger items up one space.
			for (int i = numItems - 1; i >= queueLoc; i--)
			{
				queueArray[i + 1] = queueArray[i];
			}

			queueArray[queueLoc] = item;
			numItems++;
		}
	}

	// Pop Method
	//		Returns the first item in the queue. If the queue is
	//		empty, returns null.
	Placeholder Pop()
	{
		if (numItems == 0)
		{
			cout << "There is nothing to Pop!" << endl;
			return NULL;
		}

		Placeholder item = queueArray[0];
		numItems--;

		// Shifts items one index down
		for (int i = 0; i < numItems; i++)
		{
			queueArray[i] = queueArray[i + 1];
		}

		return item;
	}

	// Print Method
	//		Prints out the elements in the queue.
	void Print()
	{
		cout << "The queue currently looks like this: " << endl;
		for (int i = 0; i < numItems; i++)
		{
			cout << queueArray[i] << " ";
		}
		cout << endl;
	}

	// Get Size Method
	//		Returns the current size of the queue.
	int GetSize()
	{
		cout << "The queue currently has " << numItems <<
			" item(s) in it." << endl;
		return numItems;
	}

	// Is Empty Method
	//		Returns whether the queue is empty or not.
	bool IsEmpty()
	{
		if (numItems == 0)
		{
			cout << "The queue contains no entries!" << endl;
			return true;
		}

		cout << "The queue has items in it." << endl;
		return false;
	}

private:
	Placeholder* queueArray;
	int queueSize;
	int numItems;
};