#include <iostream>
#include <ctime>
#include <cstdlib>
#include "STRUCT.h"

using namespace std;

//Class stack that stacks numbers(or in this case, flavors)
class Stack
{
	private:
		node *top;
		int qty;
		int max;
		
	public:
		Stack();
		Stack(int);
		bool push(int);
		bool pop(int&);
		bool peek(int&);
		bool isFull()	const;
		bool isEmpty() const;
		bool makeEmpty();
		int getQty()	const;
		~Stack();
};

Stack::Stack()
{
	top = NULL;
	qty = 0;
	max = 12;
}

Stack::Stack(int maxVal)
{
	top = NULL;
	qty = 0;
	if (maxVal < 1)
		max = 12;
	else
		max = maxVal;
}
//Adds to the stack of values
bool Stack::push(int val)
{
	if (isFull())
		return false;
	node* temp = new node;
	temp->num = val;
	temp->next = top;
	top = temp;
	qty++;
	return true;
}
//reports the top value on the stack
bool Stack::peek(int& val)
{
	if (isEmpty())
		return false;
	val = top->num;
	return true;
}
//Deletes the top most value
bool Stack::pop(int& val)
{
	if (isEmpty())
		return false;
	node* temp = top;
	val = top->num;
	top = top->next;
	delete temp;
	qty--;
	return true;
}
//Sees if the stack is full
bool Stack::isFull() const 
{
	return qty >= max;
}
//Checks to see if the stack is empty
bool Stack::isEmpty() const
{
	return top == NULL;
}
//Makes the stack empty
bool Stack::makeEmpty()
{
	while(!isEmpty())
	{
		node* temp = top;
		top = top->next;
		delete temp;
	}
	qty = 0;
	return true;
}
//Returns the qty of values in the stack 
int Stack::getQty() const
{
	return qty;
}

Stack::~Stack()
{
	makeEmpty();
}