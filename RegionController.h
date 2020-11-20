#pragma once

#include "GroupController.h"

class RegionController {
    private:
        GroupController groups[4];
    public:
        RegionController();
        ~RegionController();
};