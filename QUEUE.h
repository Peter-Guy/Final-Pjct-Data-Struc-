#include<iostream>
#include "DATA.h"
#include "NODE.h"

#ifndef QUEUE_H
#define QUEUE_H
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
		bool enqueue(int,data&);
		bool dequeue(int&, data&);
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
bool queue::enqueue(int num, data& data_d)
{
	if(isFull())
		return false;
	
	node* temp = new node;
	temp->num = num;
	temp->data_d = data_d;
	temp->next = NULL;
	qty++;
	
	if(isEmpty())
		in = out = temp;
	else
		in = in-> next = temp;
	return true;
}
//Leaves the queue
bool queue::dequeue(int & num, data& data_d)
{
	node* temp;
	if(isEmpty())
		return false;
	data_d = out->data_d;
	num = out->num;
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
		int num;
		data temp_data;
		dequeue(num, temp_data);	
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