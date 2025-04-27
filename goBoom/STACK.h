#ifndef STACK_H
#define STACK_H

#include <climits>
#include "goBoom/deck.h"
using namespace std;

#ifndef LINKED_NODE
#define LINKED_NODE

struct node {
    //Card number in hand
	//Unused in stack
    int cardNum;
    //Card in hand
    card _c;
    //Next card
    node *next;
};

#endif

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
		bool push(card&);
		bool pop(card&);
		bool peek(card&);
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
	max = INT_MAX;
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
bool Stack::push(card &c)
{
	if (isFull())
		return false;
	node* temp = new node;
	temp->_c = c;
	temp->next = top;
	top = temp;
	qty++;
	return true;
}
//reports the top value on the stack
bool Stack::peek(card &c)
{
	if (isEmpty())
		return false;
	c = top->_c;
	return true;
}
//Deletes the top most value
bool Stack::pop(card& c)
{
	if (isEmpty())
		return false;
	node* temp = top;
	c = top->_c;
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

#endif