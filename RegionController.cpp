#include <iostream>

#include "RegionController.h"

// Default Constructor
RegionController::RegionController() {
    for (int i = 0; i < 4; i++) {
        this->groups[i] = GroupController(this);
    }
}

RegionController::RegionController(DatacenterController* dcController) {
    this->parent = dcController;

    for (int i = 0; i < 4; i++) {
        this->groups[i] = GroupController(this);
    }
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

void RegionController::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for(int i = 0; i < 4; i++) {
        if (!this->groups[i].isFull()) {
            this->groups[i].fillVehicles(shiftToReplace, vehicles, time);
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

void RegionController::work() {
    for (int i = 0; i < 4; i++) {
        this->groups[i].work();
    }
}

int RegionController::getNumBusy() {
    int sumBusy = 0;
    
    for (int i = 0; i < 4; i++) {
        sumBusy += this->groups[i].getNumBusy();
    }

    return sumBusy;
}

Vehicle* RegionController::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 3);

    return this->groups[random(generator)].getRandomVehicle();
}