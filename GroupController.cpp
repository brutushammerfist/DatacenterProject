#include <iostream>

#include "GroupController.h"

// Default Constructor
GroupController::GroupController() {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i] = AccessPoint(this);
    }
}

GroupController::GroupController(RegionController* rgController) {
    this->parent = rgController;

    for (int i = 0; i < 4; i++) {
        this->accessPoints[i] = AccessPoint(this);
    }
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

void GroupController::fillVehicles(int shiftToReplace, std::list<Vehicle*> & vehicles, int time) {
    for(int i = 0; i < 4; i++) {
        if (!this->accessPoints[i].isFull()) {
            this->accessPoints[i].fillVehicles(shiftToReplace, vehicles, time);
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

void GroupController::work() {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i].work();
    }
}

int GroupController::getNumBusy() {
    int sumBusy = 0;
    
    for (int i = 0; i < 4; i++) {
        sumBusy += this->accessPoints[i].getNumBusy();
    }

    return sumBusy;
}

Vehicle* GroupController::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 39);

    return this->accessPoints[random(generator)].getRandomVehicle();
}