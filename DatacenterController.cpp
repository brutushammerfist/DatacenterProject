#include <iostream>
#include <random>

#include "DatacenterController.h"

// Default Constructor
DatacenterController::DatacenterController() {
    for (int i = 0; i < 7680; i++) {
        vehicles.push_back(new Vehicle(i));
    }

    this->initializeParkingLot();

    this->shiftToReplace = 0;
}

// Default Destructor
DatacenterController::~DatacenterController() {

}

// Check if the datacenter is currently full of vehicles
bool DatacenterController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->regions[i].isFull() == false) {
            return false;
        }
    }

    return true;
}

// Fill parking lot at beginning of simulation
void DatacenterController::initializeParkingLot() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].initializeParkingLot(this->vehicles);
    }
}

// Fill any empty parking spots with vehicles
void DatacenterController::fillVehicles() {
    for(int i = 0; i < 4; i++) {
        if (!this->regions[i].isFull()) {
            this->regions[i].fillVehicles(this->shiftToReplace, this->vehicles);
        }
    }
}

// Display every vehicle within the datacenter
void DatacenterController::display() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].display();
    }
}

// Make a shift change, swapping 320 vehicles currently in the datacenter with fresh vehicles
void DatacenterController::shiftChange() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].shiftChange(this->shiftToReplace, this->vehicles);
    }

    this->fillVehicles();
    
    if (this->shiftToReplace == 23) {
        this->shiftToReplace = 0;
    } else {
        this->shiftToReplace++;
    }
}

void DatacenterController::work() {
    for (int i = 0; i < 4; i++) {
        this->regions[i].work();
    }
}

Vehicle* DatacenterController::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 3);

    return this->regions[random(generator)].getRandomVehicle();
}