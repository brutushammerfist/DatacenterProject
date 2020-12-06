#include "VirtualMachine.h"

// Default Constructor
VirtualMachine::VirtualMachine() {
    this->size = 500;
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
    this->job = job;
}

int VirtualMachine::migrateSize() {
    return 500 + this->job->getInputSize();
}