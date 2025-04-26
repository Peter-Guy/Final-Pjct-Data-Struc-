#ifndef USLL
#define USLL

#include <climits>
#include <iostream>
#include "deck.h"
using namespace std;

#ifndef LINKED_NODE
#define LINKED_NODE

struct node {
    //Card number in hand
    int cardNum;
    //Card in hand
    card _c;
    //Next card
    node *next;
};

#endif

class usll {
    public:
        usll();
        usll(int);
        bool addNode(card&);
        bool deleteNode(const int&, card&);
        bool find(const int&, card&);
        bool makeEmpty();
        int getLength();
        bool isEmpty();
        bool isFull();
        ~usll();

    private:
        node *start;
        node *end;
        int qty;
        int max;
};

//Default constructor for Unsorted Linked List
//Uses system max integer for max length
usll::usll() {
    start = nullptr;
    end = nullptr;
    qty = 0;
    max = INT_MAX;
}

//Overloaded constructor for Unsorted Linked List
//Uses provided value or system max integer for max length
usll::usll(int _max) {
    start = nullptr;
    end = nullptr;
    qty = 0;
    if (_max < 1) max = INT_MAX;
    else max = _max;
}

//Adds a new node to the list
bool usll::addNode(card &c) {
    node *temp;
    
    //If the list is full
    if(isFull()) return false;

    temp = new node;
    temp->cardNum = ++qty;
    temp->_c = c;
    temp->next = nullptr;

    //If the list is empty
    if(isEmpty()) {
        start = temp;
        end = start;
        return true;
    }

    end->next = temp;
    end = temp;
    return true;
}

//Deletes a node from the list
bool usll::deleteNode(const int &num, card &c) {
    node *temp, *scan;
    
    //If the list is empty
    if(isEmpty()) return false;

    if (num > qty || num < 1) return false;

    scan = start;
    
    if (scan->cardNum == num) {
        qty--;

        start = start->next;

        c = scan->_c;

        delete scan;

        if (start == nullptr) {
            end = nullptr;
            return true;
        }

        for (scan = start; scan != nullptr; scan = scan->next) {
            scan->cardNum--;
        }

        return true;
    }

    for (; scan->next != nullptr; scan = scan->next) {
        if (scan->next->cardNum == num) {
            qty--;
            
            temp = scan->next;

            scan->next = temp->next;

            c = temp->_c;

            delete temp;

            if (scan->next == nullptr) {
                end = scan;
                return true;
            }
        }
        if (scan->next->cardNum > num) {
            scan->next->cardNum--;
        }
    }

    return true;
}

//Check if a value is in the list
bool usll::find(const int &num, card &c) {
    if(isEmpty() || num > qty || num < 1) return false;
    
    //Increment through all values in the list
    for(node *scan = start; scan != nullptr; scan = scan->next) {
        if (scan->cardNum == num) {
            c = scan->_c;
            return true;
        }
    }

    return false;
}

//Delete all nodes in the list and reset qty
bool usll::makeEmpty() {
    //Increment through all values in the list
    for(node *scan = start; scan != nullptr; scan = start) {
        start = start->next;
        delete scan;
    }

    end = nullptr;
    qty = 0;
    
    return true;
}

//Return the current value in qty
int usll::getLength() {
    return qty;
}

//Checks if the list is empty based on start being null
bool usll::isEmpty() {
    return start == nullptr;
}

//Checks if the list is full based on quantity being equal to max
bool usll::isFull() {
    return qty == max;
}

//Destructor for Unsorted Linked List
//Empties the list before being destroyed
usll::~usll() {
    makeEmpty();
}

#endif