#pragma once

#include "AccessPoint.h"
#include "SubJob.h"

class AccessPoint;
class DatacenterController;
class SubJob;
class Vehicle;

class VirtualMachine {
    private:
        SubJob* job;
        int size;
    public:
        VirtualMachine();
        ~VirtualMachine();

        void work(DatacenterController* dcController, AccessPoint* acPoint, Vehicle* hostVehicle, int time);

        void setJob(SubJob* job);

        int migrateSize();

        void restartJob();
};