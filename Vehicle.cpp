#include <iostream>

#include "Vehicle.h"

// Default Constructor
Vehicle::Vehicle() {
    this->id = -1;
    this->isBusy = false;
    this->vm = nullptr;
}

// Constructor with ID as parameter
Vehicle::Vehicle(int id) {
    this->id = id;
    this->isBusy = false;
    this->vm = nullptr;
}

// Default Destructor
Vehicle::~Vehicle() {

}

void Vehicle::display() {
    std::cout << "ID: " << this->id % 24 << "\n";
}

int Vehicle::getShift() {
    return this->id % 24;
}