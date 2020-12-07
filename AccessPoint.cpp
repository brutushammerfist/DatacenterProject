#include <iostream>
#include <random>

#include "AccessPoint.h"

// Default Constructor
AccessPoint::AccessPoint() {
    for (int i = 0; i < 40; i++) {
        this->cluster[i] = nullptr;
    }

    this->bandwidthUser = nullptr;
}

// Default Destructor
AccessPoint::~AccessPoint() {

}

bool AccessPoint::isFull() {
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i] == nullptr) {
            return false;
        }
    }

    return true;
}

void AccessPoint::initializeParkingLot(std::list<Vehicle*> &vehicles) {
    std::list<Vehicle*>::iterator vitr = vehicles.begin();

    for (int i = 0; i < 40; i++) {
        while (this->cluster[i] == nullptr) {
            if ((*vitr)->getShift() >= 0 and (*vitr)->getShift() < 8) {
                this->cluster[i] = (*vitr);
                vitr = vehicles.erase(vitr);
            } else {
                vitr++;
            }
        }
    }
}

void AccessPoint::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles) {
    int shiftToPark = (shiftToReplace + 8) % 24;

    std::list<Vehicle*>::iterator vitr = vehicles.begin();

    for (int i = 0; i < 40; i++) {
        while(this->cluster[i] == nullptr) {
            if ((*vitr)->getShift() == shiftToPark) {
                this->cluster[i] = (*vitr);
                vitr = vehicles.erase(vitr);
            } else {
                vitr++;
            }
        }
    }
}

void AccessPoint::display() {
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i] == nullptr) {
            std::cout << "Empty.\n";
        } else {
            this->cluster[i]->display();
        }
    }
}

void AccessPoint::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i]->getShift() == shiftToReplace) {
            vehicles.push_back(this->cluster[i]);
            this->cluster[i]->leave();
            this->cluster[i] = nullptr;
        }
    }
}

Vehicle* AccessPoint::getBandwidthUser() {
    return this->bandwidthUser;
}

void AccessPoint::work(DatacenterController* dcController, int time) {
    for (int i = 0; i < 40; i++) {
        this->cluster[i]->work(dcController, this, time);
    }
}

Vehicle* AccessPoint::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 39);

    return this->cluster[random(generator)];
}

void AccessPoint::swapBandwidthUser() {
    this->bandwidthUser = nullptr;
    
    if (!this->bandwidthQueue.empty()) {
        this->bandwidthUser = this->bandwidthQueue.front();
        this->bandwidthQueue.pop();
    }
}