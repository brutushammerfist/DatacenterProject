#include "VirtualMachine.h"

// Default Constructor
VirtualMachine::VirtualMachine() {
    this->size = 500;
}

// Default Destructor
VirtualMachine::~VirtualMachine() {

}

void VirtualMachine::work() {
    if (this->job != nullptr) {
        this->job->work();
    }
}