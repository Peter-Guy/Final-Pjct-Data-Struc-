#ifndef QUEUE_H
#define QUEUE_H

#include <climits>
#include "goBoom/deck.h"
using namespace std;

#ifndef LINKED_NODE
#define LINKED_NODE

struct node {
    //Card number in hand
	//Unused in queue
    int cardNum;
    //Card in hand
    card _c;
    //Next card
    node *next;
};

#endif

//Class that is a queue that is like a line 
class queue
{
	private:
		node* in;
		node* out;
		int qty;
		int max;
		
	public:
		queue();
		queue(int);
		bool enqueue(card&);
		bool dequeue(card&);
		bool makeEmpty();
		bool isEmpty();
		bool isFull();
		int getQty(); 
		~queue();
};

queue::queue()
{
	in = out = NULL;
	qty = 0;
	max = INT_MAX;
}

queue::queue(int num)
{
	in = out = NULL;
	qty = 0;
	if(num > 1)
		max = num;
	else
		max = 1;
}
//Enter the queue of values
bool queue::enqueue(card &c)
{
	if(isFull())
		return false;
	
	node* temp = new node;
	temp->_c = c;
	temp->next = NULL;
	qty++;
	
	if(isEmpty())
		in = out = temp;
	else
		in = in-> next = temp;
	return true;
}
//Leaves the queue
bool queue::dequeue(card &c)
{
	node* temp;
	if(isEmpty())
		return false;
	c = out->_c;
	qty--;
	temp = out;
	out = out->next;
	if(out == NULL)
		in = out;
	delete temp;
	return true;
}
//checks if queue is empty
bool queue::isEmpty()
{
	if(in == NULL && out == NULL)
		return true;
	return false;
}
//checks if queue is full
bool queue::isFull()
{
	if(qty == max)
		return true;
	return false;
}
//Makes the queue empty
bool queue::makeEmpty()
{
	while(!isEmpty())
	{
		card temp;
		dequeue(temp);	
	}
	return true;
}
//returns qty of items in que
int queue::getQty()
{
	return qty;
}

queue::~queue()
{
	makeEmpty();
}

#endif