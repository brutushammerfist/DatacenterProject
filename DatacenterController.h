#pragma once

#include <list>

#include "JobManager.h"
#include "RegionController.h"

class RegionController;
class JobManager;

class DatacenterController {
    private:
        RegionController* regions[4];
        std::list<Vehicle*> vehicles;
        int shiftToReplace;

        JobManager* jobManager;
    public:
        DatacenterController();
        ~DatacenterController();
        DatacenterController(int numReducers);

        bool isFull();

        void initializeParkingLot();

        void fillVehicles();

        void display();

        void shiftChange();

        void work(int currTime);

        Vehicle* getRandomVehicle(bool notBusy);

        void scheduleReduce(MapReduceJob* job);

        void initializeJobs();
};