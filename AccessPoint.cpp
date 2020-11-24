#include <iostream>

#include "AccessPoint.h"

// Default Constructor
AccessPoint::AccessPoint() {
    for (int i = 0; i < 40; i++) {
        this->cluster[i] = nullptr;
    }
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

/*void AccessPoint::initializeParkingLot(std::list<Vehicle*> & vehicles) {
    std::list<Vehicle*>::iterator vitr = vehicles.begin();
}

void AccessPoint::fillVehicles(std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i] == nullptr) {
            this->cluster[i] = vehicles.front();
            vehicles.pop_front();
            //vehicles.push_back(this->cluster[i]);
        }
    }
}*/

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

void AccessPoint::shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles) {
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i]->getShift() == shiftToReplace) {
            vehicles.push_back(this->cluster[i]);
            this->cluster[i] = nullptr;
        }
    }
}