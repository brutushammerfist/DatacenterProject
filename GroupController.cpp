#include <iostream>

#include "GroupController.h"

// Default Constructor
GroupController::GroupController() {

}

// Default Destructor
GroupController::~GroupController() {

}

bool GroupController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->accessPoints[i].isFull() == false) {
            return false;
        }
    }

    return true;
}

void GroupController::initializeParkingLot(std::list<Vehicle*> & vehicles) {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i].initializeParkingLot(vehicles);
    }
}

void GroupController::fillVehicles(int shiftToReplace, std::list<Vehicle*> & vehicles) {
    for(int i = 0; i < 4; i++) {
        if (!this->accessPoints[i].isFull()) {
            this->accessPoints[i].fillVehicles(shiftToReplace, vehicles);
        }
    }
}

void GroupController::display() {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i].display();
    }
}

void GroupController::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i].shiftChange(shiftToReplace, vehicles);
    }
}