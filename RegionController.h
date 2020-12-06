#pragma once

#include "GroupController.h"
#include "Vehicle.h"

class DatacenterController;
class GroupController;

class RegionController {
    private:
        GroupController* groups[4];
    public:
        RegionController();
        ~RegionController();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void work(DatacenterController* dcController, int time);

        Vehicle* getRandomVehicle();
};