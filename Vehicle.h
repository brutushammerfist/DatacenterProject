#pragma once

#include <list>

#include "AccessPoint.h"
#include "SubJob.h"
#include "VirtualMachine.h"

class AccessPoint;
class DatacenterController;
class MapReduceJob;
class SubJob;
class VirtualMachine;

class Vehicle {
    private:
        int id;
        bool isBusy;
        VirtualMachine* vm;
        std::list<MapReduceJob*> savedJobs;

        int arrival;
        int departure;

        int currMigrated;
        bool migrating;
        Vehicle* migrationTarget;
    public:
        Vehicle();
        Vehicle(int id);
        ~Vehicle();

        void display();

        int getShift();

        void work(DatacenterController* dcController, AccessPoint* acPoint, int time, int migrationType);

        bool isMigrating();

        void setJob(SubJob* job);

        void setBusy(bool busy);

        void saveJob(MapReduceJob* job);

        bool busy();

        void migrate();

        void migrateVM(VirtualMachine* vm, std::list<MapReduceJob*> &jobs);

        void setMigrate(bool migrate);

        int getDeparture();

        void leave();

        int getTimeUntilDeparture(int currTime);

        int getMigrateSize();

        void setMigrationTarget(Vehicle* target);
};