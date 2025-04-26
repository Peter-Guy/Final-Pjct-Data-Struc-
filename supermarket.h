#ifndef SUPERMARKET_H
#define SUPERMARKET_H

#include <iostream>  
#include <queue>     
#include <stack>     
#include <set>       
#include <string>    
#include <cstdlib>   // rand()

enum Product {
    EGGS,
    MILK,
    FLOUR,
    BREAD,
    WATER,
    BANANAS,
    PRODUCT_COUNT // Number of product types
};

const std::string productNames[] = {"Eggs", "Milk", "Flour", 
                                    "Bread", "Water", "Bananas"};

struct ProductItem {
    Product type;
    int quantity;
};

struct SimulationParams {
    int numRegisters;
    int minInitialCustomers;
    int maxInitialCustomers;
    int minCustomerItems;
    int maxCustomerItems;
};

class Customer {
private:
    std::stack<ProductItem> products;

public:
    // Constructor for rand products
    Customer(int minItems, int maxItems) {
        int numItems = rand() % (maxItems - minItems + 1) + minItems;
        std::set<Product> selectedProducts;
        while (selectedProducts.size() < numItems) {
            Product p = static_cast<Product>(rand() % PRODUCT_COUNT);
            if (selectedProducts.find(p) == selectedProducts.end()) {
                int quantity = rand() % 5 + 1;
                products.push({p, quantity});
                selectedProducts.insert(p);
            }
        }
    }

    void displayProducts() const {
        std::stack<ProductItem> temp = products;
        while (!temp.empty()) {
            ProductItem item = temp.top();
            std::cout << productNames[item.type] << ": " 
                      << item.quantity << std::endl;
            temp.pop();
        }
    }
};

class Register {
private:
    std::queue<Customer*> line;
    int registerNumber;

public:
    
    Register(int num) : registerNumber(num) {}

    // Destructor 
    ~Register() {
        while (!line.empty()) {
            delete line.front();
            line.pop();
        }
    }

    void addCustomer(Customer* customer) {
        line.push(customer);
    }

    void checkout() {
        if (!line.empty()) {
            Customer* customer = line.front();
            std::cout << "Register " << registerNumber 
                      << " checking out customer with products:" <<std::endl;
            customer->displayProducts();
            delete customer;
            line.pop();
        } else {
            std::cout << "Register " << registerNumber 
                      << " has no customers." << std::endl;
        }
    }

    int getSize() const {
        return line.size();
    }
};

#endif