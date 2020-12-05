#pragma once

#include "MapReduceJob.h"

class SubJob;

class VirtualMachine {
    private:
        SubJob* job;
        int size;
    public:
        VirtualMachine();
        ~VirtualMachine();

        void work();
};