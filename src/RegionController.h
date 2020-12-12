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

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void work(DatacenterController* dcController, int time, int migrationType);

        Vehicle* getRandomVehicle();

        Vehicle* findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime);
};