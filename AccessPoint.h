#pragma once

#include <queue>

#include "DatacenterController.h"
#include "Vehicle.h"

class AccessPoint {
    private:
        GroupController* parent;

        Vehicle* cluster[40];

        std::queue<Vehicle*> bandwidthQueue;
        Vehicle* bandwidthUser;
    public:
        AccessPoint();
        AccessPoint(GroupController* gpController);
        ~AccessPoint();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void work();

        bool isBusy(float percentage);

        int getNumBusy();

        Vehicle* getRandomVehicle();
};