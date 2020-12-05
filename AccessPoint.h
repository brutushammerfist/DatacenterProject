#pragma once

#include "Vehicle.h"

class AccessPoint {
    private:
        Vehicle* cluster[40];

        Vehicle* bandwidthUser;
    public:
        AccessPoint();
        ~AccessPoint();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);

        Vehicle* getBandwidthUser();

        void work();

        Vehicle* getRandomVehicle();
};