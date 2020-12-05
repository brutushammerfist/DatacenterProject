#pragma once

#include <list>

#include "DatacenterController.h"
#include "MapReduceJob.h"
#include "VirtualMachine.h"

class MapReduceJob;
class VirtualMachine;

class Vehicle {
    private:
        int id;
        bool isBusy;
        
        bool uploading;

        VirtualMachine* vm;

        std::list<MapReduceJob*> savedJobs;

        int arrival;
        int departure;

        bool finishedMigrating;
        Vehicle* migrationTarget;
        int incomingMigrationSize;

        Vehicle* clusterIntermediateTarget;
        Vehicle* randomIntermediateTarget;

        int intermedOneSaveSize;
        int intermedTwoSaveSize;
    public:
        Vehicle();
        Vehicle(int id);
        ~Vehicle();

        void display();

        int getShift();

        void setArrival(int time);

        void setDeparture(int time);

        int getDeparture();

        bool hasVM();

        bool busy();

        void leave();

        void work();

        bool isMigratingVM();

        bool uploadToDatacenter(DatacenterController* dcController);

        bool networkWork(DatacenterController* dcController, AccessPoint* acPoint);

        void migrate();

        bool isFinishedMigrating();

        void setVM(VirtualMachine* vm);

        bool isUploading();

        bool saveIntermediate(DatacenterController* dcController, AccessPoint* acPoint);

        void saveJob(MapReduceJob* job);
};