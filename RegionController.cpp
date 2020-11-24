#include <iostream>

#include "RegionController.h"

// Default Constructor
RegionController::RegionController() {

}

// Default Destructor

RegionController::~RegionController() {

}

bool RegionController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->groups[i].isFull() == false) {
            return false;
        }
    }

    return true;
}

void RegionController::initializeParkingLot(std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 4; i++) {
        this->groups[i].initializeParkingLot(vehicles);
    }
}

void RegionController::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles) {
    for(int i = 0; i < 4; i++) {
        if (!this->groups[i].isFull()) {
            this->groups[i].fillVehicles(shiftToReplace, vehicles);
        }
    }
}

void RegionController::display() {
    for (int i = 0; i < 4; i++) {
        this->groups[i].display();
    }
}

void RegionController::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 4; i++) {
        this->groups[i].shiftChange(shiftToReplace, vehicles);
    }
}