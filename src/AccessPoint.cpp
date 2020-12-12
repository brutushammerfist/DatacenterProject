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
            if ((*vitr)->getShift() >= 0 && (*vitr)->getShift() < 8) {
                this->cluster[i] = (*vitr);
                vitr = vehicles.erase(vitr);
                this->cluster[i]->setDeparture(3600 * (this->cluster[i]->getShift() + 1));
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
                this->cluster[i]->setArrival(time);
                this->cluster[i]->setDeparture(time + 28800);
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
    //Check and clear nulls in queue
    if (this->bandwidthUser != nullptr) {
        
        std::queue<Vehicle*> tempQueue;

        while (!this->bandwidthQueue.empty()) {
            Vehicle* vehicle = this->bandwidthQueue.front();
            this->bandwidthQueue.pop();
            if (vehicle->getVM() != nullptr) {
                tempQueue.push(vehicle);
            }
        }

        this->bandwidthQueue = tempQueue;
        
        if (this->bandwidthUser->getVM() == nullptr) {
            this->swapBandwidthUser();
        }
    }
    
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
    if ((this->etaQueueEmpty() + timeUntilCompletion + (int)(dataSize / 54) + 1) > 28800) {
        return nullptr;
    }
        
    for (int i = 0; i < 40; i++) {
        if ((this->etaQueueEmpty() + timeUntilCompletion + (int)(dataSize / 54) + 1) < (this->cluster[i]->getDeparture() - currTime)) {
            return this->cluster[i];
        }
    }

    return nullptr;
}

int AccessPoint::etaQueueEmpty() {
    int eta = 0;

    if (this->bandwidthUser != nullptr) {
        if (this->bandwidthUser->getVM()->getJob()->migrating()) {
            eta += (int)(this->bandwidthUser->getMigrateSize() / 54) + 1;
        } else {
            eta += (int)(this->bandwidthUser->getUploadSize() / 54) + 1;
        }
        std::list<Vehicle*>::iterator vitr = this->queueContents.begin();

        while (vitr != this->queueContents.end()) {
            if ((*vitr)->getVM() != nullptr) {
                if ((*vitr)->getVM()->getJob()->migrating()) {
                    eta += (int)((*vitr)->getMigrateSize() / 54) + 1;
                } else {
                    eta += (int)(this->bandwidthUser->getUploadSize() / 54) + 1;
                }
            }

            vitr++;
        }
    }

    return eta;
}