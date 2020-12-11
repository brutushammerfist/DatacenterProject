#pragma once

#include "AccessPoint.h"
#include "Vehicle.h"

class AccessPoint;
class DatacenterController;

class GroupController {
    private:
        AccessPoint* accessPoints[4];
    public:
        GroupController();
        ~GroupController();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        void work(DatacenterController* dcController, int time, int migrationType);

        Vehicle* getRandomVehicle();

        Vehicle* findMigrationMatch(int timeUntilCompletion, int dataSize, int currTime);
};