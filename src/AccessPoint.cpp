#include <ctime>
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

void AccessPoint::work(DatacenterController* dcController, int time, int migrationType) {
    for (int i = 0; i < 40; i++) {
        this->cluster[i]->work(dcController, this, time, migrationType);
    }
}

Vehicle* AccessPoint::getRandomVehicle() {
    return this->cluster[(rand() % 40)];
}

void AccessPoint::swapBandwidthUser() {
    this->bandwidthUser = nullptr;
    
    if (!this->bandwidthQueue.empty()) {
        this->bandwidthUser = this->bandwidthQueue.front();
        queueContents.remove(bandwidthQueue.front());
        this->bandwidthQueue.pop();
    }
}

void AccessPoint::addToBandwidthQueue(Vehicle* vehicle) {
    std::list<Vehicle*>::iterator vitr = this->queueContents.begin();

    while (vitr != this->queueContents.end()) {
        if (vehicle == (*vitr)) {
            return;
        }
        vitr++;
    }
    
    if (this->bandwidthUser != vehicle) {
        this->bandwidthQueue.push(vehicle);
        this->queueContents.push_back(vehicle);
    }

    if (this->bandwidthUser == nullptr) {
        this->swapBandwidthUser();
    }
}

void AccessPoint::printQueue() {
    std::cout << "Queue Size: " << this->queueContents.size() << std::endl;

    std::list<Vehicle*>::iterator vitr = this->queueContents.begin();

    while (vitr != this->queueContents.end()) {
        std::cout << (*vitr) << std::endl;
        vitr++;
    }

    std::cout << "Current Bandwidth User: " << this->bandwidthUser << std::endl;
}

Vehicle* AccessPoint::findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime) {
    Vehicle* vehicle = nullptr;

    if ((this->etaQueueEmpty() + timeUntilCompletion + (int)(dataSize / 54) + 1) > 28800) {
        return vehicle;
    }
        
    for (int i = 0; i < 40; i++) {
        if ((this->etaQueueEmpty() + timeUntilCompletion + (int)(dataSize / 54) + 1) < (this->cluster[i]->getDeparture() - currTime)) {
            return this->cluster[i];
        }
    }

    return vehicle;
}

int AccessPoint::etaQueueEmpty() {
    int eta = 0;

    if (this->bandwidthUser != nullptr) {
        eta += (int)(this->bandwidthUser->getMigrateSize() / 54) + 1;
    }

    std::list<Vehicle*>::iterator vitr = this->queueContents.begin();

    while (vitr != this->queueContents.end()) {
        eta += (int)((*vitr)->getMigrateSize() / 54) + 1;

        vitr++;
    }

    return eta;
}