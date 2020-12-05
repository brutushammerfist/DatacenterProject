#pragma once

#include "MapReduceJob.h"

class SubJob;

class VirtualMachine {
    private:
        SubJob* job;
        int size;
        bool isMigrating;
    public:
        VirtualMachine();
        ~VirtualMachine();

        bool migrating();

        void leave();

        void work();

        SubJob* getJob();
};