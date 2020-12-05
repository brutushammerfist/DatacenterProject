#pragma once

#include "DatacenterController.h"
#include "GroupController.h"

class RegionController {
    private:
        DatacenterController* parent;

        GroupController groups[4];
    public:
        RegionController();
        RegionController(DatacenterController* dcController);
        ~RegionController();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void work();
        
        int getNumBusy();

        DatacenterController* getParent();

        Vehicle* getRandomVehicle();
};