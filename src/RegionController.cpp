#include <ctime>
#include <iostream>

#include "RegionController.h"

// Default Constructor
RegionController::RegionController() {
    for (int i = 0; i < 4; i++) {
        this->groups[i] = new GroupController();
    }
}

// Default Destructor

RegionController::~RegionController() {

}

bool RegionController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->groups[i]->isFull() == false) {
            return false;
        }
    }

    return true;
}

void RegionController::initializeParkingLot(std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 4; i++) {
        this->groups[i]->initializeParkingLot(vehicles);
    }
}

void RegionController::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for(int i = 0; i < 4; i++) {
        if (!this->groups[i]->isFull()) {
            this->groups[i]->fillVehicles(shiftToReplace, vehicles, time);
        }
    }
}

void RegionController::display() {
    for (int i = 0; i < 4; i++) {
        this->groups[i]->display();
    }
}

void RegionController::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for (int i = 0; i < 4; i++) {
        this->groups[i]->shiftChange(shiftToReplace, vehicles, time);
    }
}

void RegionController::work(DatacenterController* dcController, int time, int migrationType) {
    for (int i = 0; i < 4; i++) {
        this->groups[i]->work(dcController, time, migrationType);
    }
}

Vehicle* RegionController::getRandomVehicle() {
    return this->groups[(rand() % 4)]->getRandomVehicle();
}

Vehicle* RegionController::findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime) {
    Vehicle* vehicle = nullptr;
        
    for (int i = 0; i < 4; i++) {
        vehicle = this->groups[i]->findMigrationMatch(timeUntilCompletion, dataSize, currTime);
        if (vehicle != nullptr) {
            return vehicle;
        }
    }

    return vehicle;
}