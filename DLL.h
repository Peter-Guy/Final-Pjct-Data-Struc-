#ifndef DLL_H
#define DLL_H

#include "DATA.h"
#include "NODE.h"

#include <iostream>
#include <climits>

//Class for a double linked list
class dll
{
	private:
		int qty;
		int max;
		node* start;
		node* curr;
	public:
	dll();
	dll(int);
	bool isFull();
	bool isEmpty();
	bool addItem(int, data&);
	bool deleteItem(int, data&);
	bool findItem(int, data&);
	void makeEmpty();
	void orderList();
	//I am using this to be able to get the start of my list
	//So that after I order my lsit, I can access
	//Start in the main cpp file to be able to print it properly 
	node* getStart() { return start; }
	~dll();
};

dll::dll()
{
	start = NULL;
	curr = NULL;
	qty = 0;
	max = INT_MAX;
}

dll::dll(int maxVal)
{
	start = NULL;
	curr = NULL;
	qty = 0;
	max = maxVal;
}
//Checks to see if the list is empty
bool dll::isEmpty()
{
	return qty == 0;
}
//Checks to see if the list is full
bool dll::isFull()
{
	return qty >= max;
}
//Finds the item requested in the file 
bool dll::findItem(int num, data& d)
{
	if(isEmpty())
		return false;
		
	if(num < curr->val)
		curr = start;
		
	do
	{
		if(curr->val == num)
		{
			d = curr->d;
			return true;
		}
		
		curr = curr->next;
		
	}while(curr != NULL && curr->val <= num);
	
	if(curr == NULL)
		curr = start;
		
	return false;
}

bool dll::addItem(int num, data& d)
{
	if(isFull())
		return false;
		
	if(isEmpty())
	{
		start = curr = new node;
		curr->prev = curr->next = NULL;
		curr->val = num;
		curr->d = d;
		qty++;
		return true;
	}
	
	if(curr->val > num)
		curr = start;
		
	while(true)
	{
		if(curr->val == num)//duplicate
			return false;
		
		if(num < curr->val)
		{
			node* temp = new node;
			temp->val = num;
			temp->d = d;
			
			if(curr-> prev != NULL)
				curr->prev->next = temp;
			else
				start = temp;
			
			temp->prev = curr->prev;
			curr->prev = temp;
			temp->next = curr;
			qty++;
			return true;
		}
		
		if(curr->next == NULL)
			break;
		curr = curr->next;
	}
	
	curr->next = new node;
	curr->next->val = num;
	curr->next->d = d;
	curr->next->next = NULL;
	curr->next->prev = curr;
	qty++;
	return true;
}
//Deletes item from list
bool dll::deleteItem(int num, data& d)
{
	node* temp;
	
	if(!findItem(num,d))
		return false;
	d = curr->d;
	
	if(curr->next == NULL && curr->prev == NULL)//One node present
	{
		delete curr;
		qty--;
		curr = start = NULL;
		return true;
	}
	
	if(curr->prev == NULL)//Start of the list
	{
		curr->next->prev = NULL;
		start = curr->next;
		delete curr;
		curr = start;
		qty--;
		return true;
	}
	
	if(curr->next == NULL)//End of the list
	{
		curr = curr->prev;
		delete curr->next;
		curr->next = NULL;
		qty--;
		return true;
	}
	
	temp = curr;
	curr->prev->next = curr->next;
	curr->prev = temp->prev;
	curr = curr->next;
	delete temp;
	qty--;
	return true;
}
//Orders the list 
void dll::orderList()
{
    if (isEmpty()) 
	{
        return;
    }

    // Bubble sort to order the list by ID
    node* i = start;
    while (i != NULL) 
	{
        node* j = i->next;
        while (j != NULL) 
		{
            if (i->val > j->val) 
			{
                
                int tempVal = i->val;
                data tempData = i->d;

                i->val = j->val;
                i->d = j->d;

                j->val = tempVal;
                j->d = tempData;
            }
            j = j->next;
        }
        i = i->next;
    }

}
//Makes the list empty
void dll::makeEmpty()
{
	node* temp;
	while(start != NULL)
	{
		temp = start;
		start = start->next;
		delete temp;
	}
	
	qty = 0;
	curr = NULL;
}

dll::~dll()
{
	makeEmpty();
}

#endif