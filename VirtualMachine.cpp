#include <iostream>

#include "VirtualMachine.h"

// Default Constructor
VirtualMachine::VirtualMachine() {
    this->size = 500;
    this->job = nullptr;
}

// Default Destructor
VirtualMachine::~VirtualMachine() {

}

VirtualMachine::VirtualMachine(SubJob* job) {
    this->size = 500;
    this->job = job;
}

void VirtualMachine::work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time, int migrationType) {
    if (this->job != nullptr) {
        this->job->work(dcController, acPoint, hostVehicle, time, migrationType);
    }
}

void VirtualMachine::setJob(SubJob* job) {
    this->job = job;
}

int VirtualMachine::migrateSize() {
    if (this->job != nullptr) {
        return 500 + this->job->getInputSize();
    }
    return 500;
}

void VirtualMachine::restartJob() {
    if (this->job != nullptr) {
        this->job->restart();
    }
}

bool VirtualMachine::hasJob() {
    if (this->job == nullptr) {
        return false;
    } else {
        return true;
    }
}