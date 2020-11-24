#pragma once

#include "MapReduceJob.h"

class MapReduceJob;

class VirtualMachine {
    private:
        MapReduceJob* job;
        int size;
    public:
        VirtualMachine();
        ~VirtualMachine();
};