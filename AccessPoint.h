#pragma once

#include <queue>

#include "Vehicle.h"

class DatacenterController;
class Vehicle;

class AccessPoint {
    private:
        Vehicle* cluster[40];

        std::queue<Vehicle*> bandwidthQueue;
        Vehicle* bandwidthUser;
    public:
        AccessPoint();
        ~AccessPoint();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> &vehicles);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles, int time);

        Vehicle* getBandwidthUser();

        void work(DatacenterController* dcController, int time);

        Vehicle* getRandomVehicle();

        void swapBandwidthUser();

        void addToBandwidthQueue(Vehicle* vehicle);
};