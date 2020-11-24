#pragma once

#include "AccessPoint.h"

class GroupController {
    private:
        AccessPoint accessPoints[4];
    public:
        GroupController();
        ~GroupController();

        bool isFull();

        void initializeParkingLot(std::list<Vehicle*> &vehicles);

        void fillVehicles(int shiftToReplace, std::list<Vehicle*> & vehicles);

        void display();

        void shiftChange(int shiftToReplace, std::list<Vehicle*> &vehicles);
};