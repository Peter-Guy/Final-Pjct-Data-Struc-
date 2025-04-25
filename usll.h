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
    //Used exclusively for card selection
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
        bool display();
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
    //The card number will be the new quantity
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

    //Invalid possible card numbers
    if (num > qty || num < 1) return false;

    //Start at the beginning of the list
    scan = start;

    //Check first card
    if (scan->cardNum == num) {
        qty--;

        start = start->next;

        //Take card out of hand
        c = scan->_c;

        delete scan;

        if (start == nullptr) {
            end = nullptr;
            return true;
        }

        //Reduce card number of all following cards
        for (scan = start; scan != nullptr; scan = scan->next) {
            scan->cardNum--;
        }

        return true;
    }

    //Check each card
    for (; scan->next != nullptr; scan = scan->next) {
        if (scan->next->cardNum == num) {
            temp = scan->next;

            scan->next = temp->next;

            if (end == temp) {
                end = scan;
            }

            c = temp->_c;

            delete temp;
        }
        //Reduce the card number of all following cards
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

//Output the values in the list
bool usll::display() {
    //If the list is empty
    if(isEmpty()) return false;
    
    //Increment through all but the last value in the list
    for(node *scan = start; scan != end; scan = scan->next) {
        cout << scan->cardNum << ": " << scan->_c.face << " " 
            << scan->_c.suit << ", ";
    }
    cout << end->cardNum << ": " << end->_c.face << " " 
        << end->_c.suit << "\n";

    return true;
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
