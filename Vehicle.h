#pragma once

#include <list>

#include "MapReduceJob.h"
#include "VirtualMachine.h"

class MapReduceJob;
class VirtualMachine;

class Vehicle {
    private:
        int id;
        bool isBusy;
        VirtualMachine* vm;
        std::list<MapReduceJob*> savedJobs;
    public:
        Vehicle();
        Vehicle(int id);
        ~Vehicle();

        void display();

        int getShift();

        void work();
};