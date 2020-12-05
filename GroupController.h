#pragma once

#include "DatacenterController.h"
#include "AccessPoint.h"

class GroupController {
    private:
        RegionController* parent;

        AccessPoint accessPoints[4];
    public:
        GroupController();
        GroupController(RegionController* rgController);
        ~GroupController();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void work();

        int getNumBusy();

        RegionController* getParent();

        Vehicle* getRandomVehicle();
};