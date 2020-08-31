#include "queue.h"

void wrapper()
{
	cout << "String Queue: " << endl;
	PriorityQueue<bool> queue;

	queue.IsEmpty();

	queue.Push(true);
	queue.Push(false);
	queue.Push(true);
	queue.Push(false);

	queue.Print();

	queue.GetSize();
	queue.IsEmpty();

	queue.Pop();

	queue.Print();
	queue.GetSize();

}

int main()
{
	wrapper();
}