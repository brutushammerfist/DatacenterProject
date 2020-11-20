#pragma once

#include "MapReduceJob.h"

class VirtualMachine {
    private:
        MapReduceJob job;
        int size;
    public:
        VirtualMachine();
        ~VirtualMachine();
};