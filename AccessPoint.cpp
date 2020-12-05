#include <iostream>

#include "AccessPoint.h"

// Default Constructor
AccessPoint::AccessPoint() {
    for (int i = 0; i < 40; i++) {
        this->cluster[i] = nullptr;
    }
}

AccessPoint::AccessPoint(GroupController* gpController) {
    this->parent = gpController;
    
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

void AccessPoint::initializeParkingLot(std::list<Vehicle*> &vehicles) {
    std::list<Vehicle*>::iterator vitr = vehicles.begin();

    for (int i = 0; i < 40; i++) {
        while (this->cluster[i] == nullptr) {
            if ((*vitr)->getShift() >= 0 and (*vitr)->getShift() < 8) {
                this->cluster[i] = (*vitr);
                vitr = vehicles.erase(vitr);
                this->cluster[i]->setDeparture(3600 + (this->cluster[i]->getShift() * 3600));
            } else {
                vitr++;
            }
        }
    }
}

void AccessPoint::fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time) {
    int shiftToPark = (shiftToReplace + 8) % 24;

    std::list<Vehicle*>::iterator vitr = vehicles.begin();

    for (int i = 0; i < 40; i++) {
        while(this->cluster[i] == nullptr) {
            if ((*vitr)->getShift() == shiftToPark) {
                this->cluster[i] = (*vitr);
                vitr = vehicles.erase(vitr);
                this->cluster[i]->setArrival(time);
                this->cluster[i]->setDeparture(time + 28800);
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

void AccessPoint::work() {
    for (int i = 0; i < 40; i++) {
        this->cluster[i]->work();
    }

    bool userDone = false;

    if (this->bandwidthQueue.empty()) {
        if (this->bandwidthUser != nullptr) {
            // Do Work.
            userDone = this->bandwidthUser->networkWork(this->parent->getParent()->getParent(), this);
        }
    } else {
        if (this->bandwidthUser == nullptr) {
            this->bandwidthUser = this->bandwidthQueue.front();
            this->bandwidthQueue.pop();
        }

        // Do Work.
        userDone = this->bandwidthUser->networkWork(this->parent->getParent()->getParent(), this);
    }

    if (userDone) {
        this->bandwidthUser = nullptr;
    }
}

bool AccessPoint::isBusy(float percentage) {
    int busy = 0;
    
    for (int i = 0; i < 40; i++) {
        if (this->cluster[i]->busy()) {
            busy++;
        }
    }

    if ((float)(busy / 40) > percentage) {
        return true;
    } else {
        return false;
    }
}

int AccessPoint::getNumBusy() {
    int sumBusy = 0;

    for (int i = 0; i < 40; i++) {
        if (this->cluster[i]->busy()) {
            sumBusy++;
        }
    }

    return sumBusy;
}

Vehicle* AccessPoint::getRandomVehicle() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random(0, 39);

    return this->cluster[random(generator)];
}