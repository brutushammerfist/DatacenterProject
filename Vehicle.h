#pragma once

#include "VirtualMachine.h"

class Vehicle {
    private:
        bool isBusy;
        VirtualMachine vm;
    public:
        Vehicle();
        ~Vehicle();
};