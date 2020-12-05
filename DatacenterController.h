#pragma once

#include <list>

#include "JobManager.h"
#include "RegionController.h"

class DatacenterController {
    private:
        RegionController regions[4];
        std::list<Vehicle*> vehicles;
        int shiftToReplace;

        JobManager jobManager;
    public:
        DatacenterController();
        ~DatacenterController();

        bool isFull();

        void initializeParkingLot();

        void fillVehicles();

        void display();

        void shiftChange();

        void work();

        Vehicle* getRandomVehicle();
};