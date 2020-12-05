#pragma once

#include <list>

#include "JobManager.h"
#include "RegionController.h"

class DatacenterController {
    private:
        JobManager jobManager;
        RegionController regions[4];
        std::list<Vehicle*> vehicles;
        int shiftToReplace;
        float percentBusyMax;
        float percentBusy;
    public:
        DatacenterController();
        DatacenterController(float percentBusy, int numReducers);
        ~DatacenterController();

        bool isFull();

        void initializeParkingLot();

        void fillVehicles(int time);

        void display();

        void shiftChange(int time);

        void work();

        float calcPercentBusy();

        void assignNewJob();

        Vehicle* findSuitableMigration();

        void receiveUpload(SubJob* job);

        Vehicle* getRandomVehicle();
};