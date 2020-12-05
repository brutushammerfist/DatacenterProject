#include <iostream>

#include "VirtualMachine.h"

// Default Constructor
VirtualMachine::VirtualMachine() {
    this->size = 500;
    this->isMigrating = false;
}

// Default Destructor
VirtualMachine::~VirtualMachine() {

}

bool VirtualMachine::migrating() {
    return this->isMigrating;
}

void VirtualMachine::leave() {
    if (!this->job->uploaded()) {
        this->job->restart();
    }
}

void VirtualMachine::work() {
    if (!this->isMigrating) {
        this->job->work();
    }
}

SubJob* VirtualMachine::getJob() {
    return this->job;
}