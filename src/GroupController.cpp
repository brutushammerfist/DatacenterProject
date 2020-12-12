#include <ctime>
#include <iostream>

#include "GroupController.h"

// Default Constructor
GroupController::GroupController() {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i] = new AccessPoint();
    }
}

// Default Destructor
GroupController::~GroupController() {

}

bool GroupController::isFull() {
    for(int i = 0; i < 4; i++) {
        if (this->accessPoints[i]->isFull() == false) {
            return false;
        }
    }

    return true;
}

void GroupController::initializeParkingLot(std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i]->initializeParkingLot(vehicles);
    }
}

void GroupController::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for(int i = 0; i < 4; i++) {
        if (!this->accessPoints[i]->isFull()) {
            this->accessPoints[i]->fillVehicles(shiftToReplace, vehicles, time);
        }
    }
}

void GroupController::display() {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i]->display();
    }
}

void GroupController::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i]->shiftChange(shiftToReplace, vehicles, time);
    }
}

void GroupController::work(DatacenterController* dcController, int time, int migrationType) {
    for (int i = 0; i < 4; i++) {
        this->accessPoints[i]->work(dcController, time, migrationType);
    }
}

Vehicle* GroupController::getRandomVehicle() {
    return this->accessPoints[(rand() % 4)]->getRandomVehicle();
}

Vehicle* GroupController::findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime) {
    Vehicle* vehicle = nullptr;
        
    for (int i = 0; i < 4; i++) {
        vehicle = this->accessPoints[i]->findMigrationMatch(timeUntilCompletion, dataSize, currTime);
        if (vehicle != nullptr) {
            return vehicle;
        }
    }

    return vehicle;
}