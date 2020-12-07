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

void VirtualMachine::work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time) {
    if (this->job != nullptr) {
        this->job->work(dcController, acPoint, hostVehicle, time);
    }
}

void VirtualMachine::setJob(SubJob* job) {
    std::cout << "VM setting job!\n";
    std::cout << this->job << "\n";
    std::cout << job << "\n";
    this->job = job; // Crashing here!
    std::cout << "VM job set!\n";
}

int VirtualMachine::migrateSize() {
    return 500 + this->job->getInputSize();
}

void VirtualMachine::restartJob() {
    this->job->restart();
}